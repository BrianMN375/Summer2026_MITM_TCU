#ifndef GLOBAL_VARS_H
#define GLOBAL_VARS_H
#include <elapsedMillis.h>
#include <FlexCAN_T4.h>
// #include <TFTCAN_SUSP_CAN.h>
// #include <TFTCAN_OBD_CAN.h>
// #include <TFTCAN_PT_CAN.h>
// #include <MQB_CANbus_ParsingHelpers.h>

#include <Arduino.h>

// #define VirginModule

#define Concept_MITM_TCU_EngTqMultiplying


extern float InternalTemp;
extern int InternalCPUTemp;
extern unsigned int CPU_SpeedMhz;
extern float CPU_Temp;



// Define Board Hardware (General)

  // #define Board_BasicDAZA_v2024x1
  // #define Board_KitchenSink_v2023x4
  // #define Board_KitchenSink_v2024x1
  // #define Board_KitchenSink_v2024x4
  // #define Board_KitchenSink_v2025x2
  // #define Board_KitchenSink_v2025x3
  #define Board_KitchenSink_v2025x4
  // #define Board_DAZA_DI_Delete_v1x2


// Define Concept

    // #define Concept_BasicDAZA
    // #define Concept_KitchenSink_NoSIC
    #define Concept_KitchenSink_SIC_Separate
    // #define Concept_KitchenSink_Sandals
    // #define Concept_SuspensionLevelSensors_Direct_BasicDAZA
    // #define Concept_TFT_Potentiometers_to_CAN_Module
    // #define Concept_TFT_StandaloneHaldexController
    

// Define Vehicle Config - OEM Hardware
    // #define SuspensionLevelSensors_Direct_KSCH
    // #define SuspensionLevelSensors_Direct_StandaloneModule_Master
    // #define KitchenSink_LevelSensors_via_DCC
    // #define AWDModule_OEMGen5Haldex
    #define AWDModule_SyvecsAWD
    // #define AWDModule_SyvecsAWD_and_KitchenSink_LevelSensors_via_DCC

    // 
    // #define TFT_DAZA_SIC_Installed
    #define TFT_DAZA_SIC_NOT_Installed
    // 
    // #define KitchenSink_2024x4_SIC_Separate
    // #define KitchenSink_2025x3_SIC_Separate
    // #define KitchenSink_2024x4_SIC_Sandals
    //
    // #define TFT_KS_EGTs_Installed
    // #define TurboSpeedSensor_Installed
    #define KS2025x4_NTC1_InUse
    // #define KS2025x4_NTC2_InUse

    // #define KS2025x4_A0v_InUse
    #define KS2025x4_A1v_InUse
  
    // #define KS2025x4_AD2v_InUse
    // #define KS2025x4_AD3v_InUse

    #define KS2025x4_AD2T_EGT_InUse
    #define KS2025x4_AD3T_EGT_InUse



    
  // Define Car Names and Configuration Attributes  
    // #define KitchenSink_Chase2024
    // #define KitchenSink_RatRS3_NoSIC
    // #define KitchenSink_BlakeRS3_2025
    #define KitchenSink_RatRS3_2025_SIC_Separate
    // #define TylerTTRS
    // #define DOC_RS3
    // #define AaronStehly_TFT_KS_HaldexController




// Define Configuration Attributes
    // #define BasicDAZA_v2x0 // IMU VLOGIC pin different from 2023+
    // #define DAZA_KitchenSink_FuelPumpRelayTriggers
    // #define Tyler2025_KitchenSink_FuelPumpRelayTriggers
    // #define DOC_BasicDAZA_SecondaryLPFP_Trigger
    #define RatRS3_BOV_ASV_viaLS3
    #define NoLPFPOutputs


#ifdef Board_BasicDAZA_v2024x1
  #define Board_Family "TFT_DAZA_BasicDAZA_2024x1"
  #define Board_Version "2024.1"
  #define Firmware_Version "2025.05.11"
#endif


#ifdef Board_KitchenSink_v2023x4
  #define Board_Family "KitchenSink"
  #define Board_Version "2023.4"
  #define Firmware_Version "2025.08.24"
#endif

#ifdef Board_KitchenSink_v2024x1
  #define Board_Family "KitchenSink"
  #define Board_Version "2024.1"
  #define Firmware_Version "2025.08.24"
#endif

#ifdef Board_KitchenSink_v2024x3
  #define Board_Family "KitchenSink"
  #define Board_Version "2024.3"
  #define Firmware_Version "2025.08.24"
#endif

#ifdef Board_KitchenSink_v2024x4
  #define Board_Family "KitchenSink"
  #define Board_Version "2024.4"
  #define Firmware_Version "2025.08.24"
#endif

#ifdef Board_KitchenSink_v2025x2
  #define Board_Family "KitchenSink"
  #define Board_Version "2025.2"
  #define Firmware_Version "2025.08.24"
#endif

#ifdef Board_KitchenSink_v2025x3
  #define Board_Family "KitchenSink"
  #define Board_Version "2025.3"
  #define Firmware_Version "2025.9.24"
#endif

#ifdef Board_KitchenSink_v2025x4
  #define Board_Family "KitchenSink"
  #define Board_Version "2025.4"
  #define Firmware_Version "2026.5.1"
#endif




#ifdef Concept_BasicDAZA
  #define Concept_Type "Concept_BasicDAZA"
#endif

#ifdef Concept_KitchenSink_SIC_Separate
  #define Concept_Type "Concept_KitchenSink_SIC_Separate"
#endif

#ifdef Concept_KitchenSink_NoSIC
  #define Concept_Type "Concept_KitchenSink_NoSIC"
#endif

#ifdef Concept_SuspensionLevelSensors_Direct_BasicDAZA
  #define Concept_Type "Concept_SuspensionLevelSensors_Direct_BasicDAZA"
#endif

#ifdef Concept_TFT_StandaloneHaldexController
  #define Concept_Type "Concept_TFT_StandaloneHaldexController"
#endif

#ifdef Concept_TFT_Potentiometers_to_CAN_Module
  #define Concept_Type "Concept_TFT_Potentiometers_to_CAN_Module using BasicDAZA"
#endif


// Attributes worth noting 

#ifdef KitchenSink_BlakeRS3_2025
  #define CarName "TFT_DAZA_KSCS_SeparateSIC_BlakeAug2025"
#endif

#ifdef KitchenSink_RatRS3_2025_SIC_Separate
  #define CarName "TFT_DAZA_KSCS_SeparateSIC_RatAug2025"
#endif

#ifdef TylerTTRS
  #define CarName "TylerTTRS"
  // #define EGT_DeltaToleranceInt_def 100
  // #define EGT_DeltaTolerancePct_def 8
#endif

#ifdef DOC_RS3
  #define CarName "DOC_RS3"
#endif

#ifdef DAZA_KitchenSink_FuelPumpRelayTriggers
  #define CarName "DAZA_KitchenSink_FuelPumpRelayTriggers"
#endif

#ifdef TylerTTRS
  #define CarName "TylerWall TTRS Sept2025"
#endif

#ifdef TFT_KS_EGTs_Installed
  #define Config_EGTs "EGTs_Installed_in_KitchenSink"
  #define EGT_DeltaToleranceInt_def 100
  #define EGT_DeltaTolerancePct_def 8

#endif

#ifndef TFT_KS_EGTs_Installed
  #define Config_EGTs "EGTs_NOT_Installed_in_KitchenSink"
#endif

#ifdef Concept_TFT_StandaloneHaldexController
  #define CarName "Generic Haldex Controller - MQB"
#endif

#ifdef Tyler2025_KitchenSink_FuelPumpRelayTriggers
  #define LPFP_OutputConfig "TylerWall Stock In-Tank, External Surge Tank.  LS1 activates Constant LPFP, LS2 and LS3 activates 2 more LPFPs, LS4 activates in-tank siphon pump.."
#endif

#ifdef NoLPFPOutputs
  #define LPFP_OutputConfig "No LPFP Outputs"
#endif





#define MQB_LaunchControl_BoostWindow_EntryThresholdPressure_Hysteresis_hPa 100
#define MQB_DefaultEngRPMLimit 8000
// #define MQB_LaunchControl_EngineRPM_Hold_TransitionRateMultiplier_Float_def 25
#define MQB_LaunchControl_EngineRPM_Hold_TransitionTimeout 2000

#define MQB_GaugeDisplayOverride_TxRate_micros 7000

#define LC_IgnOffsetBase_def -18

// #define MQB_AutoShift_EngineRPM_KalmanFilter_Gain_Float_def 0.5
#define MQB_AutoShift_EngineRPM_KalmanFilteredMPC_Gain_Float_def 0.5

// PID Controller Outputs
extern double input;
extern double output;
extern signed int outputIntSigned;

// Arbitrary setpoint and gains - adjust these as fit for your project:
extern double setpoint;
extern double p;
extern double i;
extern double d;

////////////////

#pragma region // Mapping of Jobs for Configurations/Settings/Service22 Functions to Integers

#define Job_MaxJobNumber 60
#define Job_BBG_multiplier_gear1 1
#define Job_BBG_multiplier_gear2 2
#define Job_BBG_multiplier_gear3 3
#define Job_BBG_multiplier_gear4 4
#define Job_BBG_multiplier_gear5 5
#define Job_BBG_multiplier_gear6 6
#define Job_BBG_multiplier_gear7 7
#define Job_BBG_Activation 8
#define Job_TBD9 9
#define Job_DTC_Clear 10
#define Job_Susp_LvlSensors_Reset 11
#define Job_LPFP_Activation 12
#define Job_DQ500_ClutchCharacteristic_Reset 13
#define Job_DQ500_DblClutch_Reset 14
#define Job_DQ500_BasicSettings_Reset 15
#define Job_DynoMode_Activation 16
#define Job_TBD17 17
#define Job_Calibrate_IMU 18
#define Job_Set_AutoShift_KalmanFilteredMPC_Gain 19
#define Job_Set_AutoShift_RPMSetpoints 20
#define Job_Set_BurnoutMode_RPM 21
#define Job_Set_LC_EngRPM_RAMP_RPM 22
#define Job_Set_LC_EngRPM_HOLD_RPM 23
#define Job_Set_LC_EngRPM_TransitionRateMultiplier 24
#define Job_Set_LC_EngRPM_PID_BaseOffset 25
#define Job_Set_LC_EngRPM_PID_Ki_Term 26
#define Job_Set_LC_EngRPM_RAMP_Mode_Timeout  27
#define Job_Set_LC_BoostVariableSelection 28
#define Job_Set_LC_BoostWindow_EntryThresholdPressure 29
#define Job_Set_LC_BoostWindow_minMaintain_Pressure 30
#define Job_Set_TC_SlipRateTarget 31
#define Job_Set_TC_CorrectionAggression 32
#define Job_Configure_LS1_Output_Type 33
#define Job_Configure_LS2_Output_Type 34
#define Job_Configure_LS3_Output_Type 35
#define Job_Configure_LS4_Output_Type 36
#define Job_Configure_LS5_Output_Type 37
#define Job_Configure_LS6_Output_Type 38
#define Job_Configure_HS1_Output_Type 39
#define Job_Configure_LS1_ActivationTimer 40
#define Job_Configure_LS2_ActivationTimer 41
#define Job_Configure_LS3_ActivationTimer 42
#define Job_Configure_LS4_ActivationTimer 43
#define Job_Configure_LS5_ActivationTimer 44
#define Job_Configure_LS6_ActivationTimer 45
#define Job_Configure_HS1_ActivationTimer 46
#define Job_Configure_LS1_PWM_Frequency 47
#define Job_Configure_LS2_PWM_Frequency 48
#define Job_Configure_LS3_PWM_Frequency 49
#define Job_Configure_LS4_PWM_Frequency 50
#define Job_Configure_LS5_PWM_Frequency 51
#define Job_Configure_LS6_PWM_Frequency 52
#define Job_Configure_HS1_PWM_Frequency 53
#define Job_Configure_Temp7_Type 54
#define Job_Configure_Temp8_Type 55
#define Job_Configure_Susp_LvlSensorType 56
#define Job_Configure_AWDControllerType 57
#define Job_Configure_DisplayGauges 58
#define Job_Set_ParachuteArmSpeed 59
// #define Job_
// #define Job_
// #define Job_
// #define Job_
// #define Job_
// #define Job_
// #define Job_
// #define Job_






#pragma endregion




#pragma region // Switches to activate/de-activate CAN Functions


#define Switch_Req_PT_CAN_09_VIN_def 0
#define Switch_Req_PT_CAN_09_CALID_def 0
#define Switch_Req_PT_CAN_09_ECUName_def 0
#define Switch_Req_OBD_CAN_09_VIN_def 0
#define Switch_Req_OBD_CAN_09_CALID_def 0
#define Switch_Req_OBD_CAN_09_ECUName_def 0



#define Switch_OBD_canTx_DS2_mux_def 1

#define Switch_Req_22_OBD_CAN_HaldexSet1_def 1
#define Switch_Req_22_OBD_CAN_LevelSensors_def 1

#define Switch_Req_22_TCU_Set1_ClutchPressureActuals_def 0

#define Switch_Req_22_OBD_CAN_TCU_AllTqsAndPressures_def 0
#define Switch_Req_22_OBD_CAN_TCU_Set1_ClutchPressureActuals_def 0 // OLD Set 1 (Pressure Actuals only) via OBD_CAN
#define Switch_Req_22_OBD_CAN_TCU_Set2_ClutchPressureTargets_def 0
#define Switch_Req_22_OBD_CAN_TCU_Set3_ClutchPlateTempActuals_def 0
#define Switch_Req_22_OBD_CAN_TCU_Set4_ClutchTorqueActuals_def 0
#define Switch_Req_22_OBD_CAN_TCU_Set5_ClutchTorqueTargets_def 0

#define Switch_Req_22_PT_CAN_TCU_AllTqsAndPressures_def 0
#define Switch_Req_22_PT_CAN_TCU_Set1_ClutchPressureActuals_def 1 // OLD Set 1 (Pressure Actuals only) via PT_CAN
#define Switch_Req_22_PT_CAN_TCU_Set2_ClutchPressureTargets_def 0
#define Switch_Req_22_PT_CAN_TCU_Set3_ClutchPlateTempActuals_def 0
#define Switch_Req_22_PT_CAN_TCU_Set4_ClutchTorqueActuals_def 0
#define Switch_Req_22_PT_CAN_TCU_Set5_ClutchTorqueTargets_def 0

#define Switch_Req_OBD_CAN_Service22_MFSW_def 0

extern unsigned int Switch_OBD_canTx_DS2_mux_Int;

extern unsigned int Switch_Req_22_OBD_CAN_HaldexSet1;
extern unsigned int Switch_Req_22_OBD_CAN_LevelSensors;
extern unsigned int Switch_Req_OBD_CAN_09_VIN;
extern unsigned int Switch_Req_OBD_CAN_09_CALID;
extern unsigned int Switch_Req_OBD_CAN_09_ECUName;

extern unsigned int Switch_Req_22_TCU_Set1_ClutchPressureActuals; // OLD TCU Set 1 (Pressures only)

extern unsigned int Switch_Req_22_OBD_CAN_TCU_AllTqsAndPressures;
extern unsigned int Switch_Req_22_OBD_CAN_TCU_Set1_ClutchPressureActuals;  // OLD Set 1 (Pressure Actuals only) via PT_CAN
extern unsigned int Switch_Req_22_OBD_CAN_TCU_Set2_ClutchPressureTargets;
extern unsigned int Switch_Req_22_OBD_CAN_TCU_Set3_ClutchPlateTempActuals;
extern unsigned int Switch_Req_22_OBD_CAN_TCU_Set4_ClutchTorqueActuals;
extern unsigned int Switch_Req_22_OBD_CAN_TCU_Set5_ClutchTorqueTargets;

extern unsigned int Switch_Req_22_PT_CAN_TCU_AllTqsAndPressures;
extern unsigned int Switch_Req_22_PT_CAN_TCU_Set1_ClutchPressureActuals; // OLD Set 1 (Pressure Actuals only) via PT_CAN
extern unsigned int Switch_Req_22_PT_CAN_TCU_Set2_ClutchPressureTargets;
extern unsigned int Switch_Req_22_PT_CAN_TCU_Set3_ClutchPlateTempActuals;
extern unsigned int Switch_Req_22_PT_CAN_TCU_Set4_ClutchTorqueActuals;
extern unsigned int Switch_Req_22_PT_CAN_TCU_Set5_ClutchTorqueTargets;




#pragma endregion


// extern unsigned int Switch_Req_OBD_Service22_MFSW; // Switch to enable/disable sending OBD Requests for Multifunction Steering Wheel 


#pragma region // Initial Identifications of VIN, CALID, ECUName, etc - via PT_CAN or OBD_CAN


  extern unsigned int CarCode; // Overall vehicle code, which is derived from Identification functions
  extern unsigned int EngineCode; // Engine Code, derived from identification functions
  extern unsigned int TransmissionCode; // Transmission Code, derived from identification functions

  extern unsigned int storedMQB_CarCode_Int;
  extern unsigned int storedMQB_EngineCode_Int;
  extern unsigned int storedMQB_TransmissionCode_Int;

  extern unsigned int initialMQB_CarCode_Int;
  extern unsigned int initialMQB_EngineCode_Int;
  extern unsigned int initialMQB_TransmissionCode_Int;
  
  extern unsigned int EEPROM_val_LSB_MQB_CarCode;
  extern unsigned int EEPROM_val_LSB_MQB_EngineCode;
  extern unsigned int EEPROM_val_LSB_MQB_TransmissionCode;

  
  extern int OBD_CAN_VIN; // service09 0A
  extern int OBD_CAN_ECU_HW_Name; // service09 0A
  extern int OBD_CAN_ECU_CALID; // service09 0A
   
  extern int OBD_CAN_TCU_HW_Name; // service09 0A
  extern int OBD_CAN_TCU_ASW; // service09 0A

  extern int PT_CAN_VIN; // service09 0A
  extern int PT_CAN_ECU_HW_Name; // service09 0A
  extern int PT_CAN_ECU_CALID; // service09 0A
   
  extern int PT_CAN_TCU_HW_Name; // service09 0A
  extern int PT_CAN_TCU_ASW; // service09 0A

   
  extern unsigned int OBD_CAN_VIN_ReadComplete;
  extern unsigned int OBD_CAN_ECU_HW_ReadComplete;
  extern unsigned int OBD_CAN_ECU_CALID_ReadComplete;
  extern unsigned int OBD_CAN_ECU_Variant4_ReadComplete;
  extern unsigned int OBD_CAN_ECU_Variant5_ReadComplete;
  extern unsigned int OBD_CAN_ECU_Variant6_ReadComplete;
  extern unsigned int OBD_CAN_ECU_Variant7_ReadComplete;
   
  extern unsigned int OBD_CAN_TCU_Variant1_ReadComplete;
  extern unsigned int OBD_CAN_TCU_Variant2_ReadComplete;
  extern unsigned int OBD_CAN_TCU_Variant3_ReadComplete;
  extern unsigned int OBD_CAN_TCU_Variant4_ReadComplete;
    
   
  extern unsigned int PT_CAN_VIN_ReadComplete;
  extern unsigned int PT_CAN_ECU_HW_ReadComplete;
  extern unsigned int PT_CAN_ECU_CALID_ReadComplete;
  extern unsigned int PT_CAN_ECU_Variant4_ReadComplete;
  extern unsigned int PT_CAN_ECU_Variant5_ReadComplete;
  extern unsigned int PT_CAN_ECU_Variant6_ReadComplete;
  extern unsigned int PT_CAN_ECU_Variant7_ReadComplete;
   
  extern unsigned int PT_CAN_TCU_Variant1_ReadComplete;
  extern unsigned int PT_CAN_TCU_Variant2_ReadComplete;
  extern unsigned int PT_CAN_TCU_Variant3_ReadComplete;
  extern unsigned int PT_CAN_TCU_Variant4_ReadComplete;


#pragma endregion

#pragma region // EEEPROM Address Definitions


#define EEPROM_address_LSB_SuspLvlSensor_FL 20
#define EEPROM_address_MSB_SuspLvlSensor_FL 21
#define EEPROM_address_LSB_SuspLvlSensor_FR 22
#define EEPROM_address_MSB_SuspLvlSensor_FR 23
#define EEPROM_address_LSB_SuspLvlSensor_RL 24
#define EEPROM_address_MSB_SuspLvlSensor_RL 25
#define EEPROM_address_LSB_SuspLvlSensor_RR 26
#define EEPROM_address_MSB_SuspLvlSensor_RR 27


#define EEPROM_address_LSB_storedMQB_CarConfig_SuspLevelSensors 28
#define EEPROM_address_LSB_storedMQB_CarConfig_AWDController 29
#define EEPROM_address_LSB_storedMQB_CarConfig_DisplayGaugesOverride 94


#define EEPROM_address_LSB_storedMQB_AutoShift_EngineRPM_KalmanFilteredMPC_Gain_Int 85 // Define EEPROM Address 
#define EEPROM_address_LSB_storedAutoUPshift_EngineRPM_Setpoint12 30 // Define EEPROM Address 
#define EEPROM_address_MSB_storedAutoUPshift_EngineRPM_Setpoint12 31 // Define EEPROM Address 
#define EEPROM_address_LSB_storedAutoUPshift_EngineRPM_Setpoint23 32 // Define EEPROM Address 
#define EEPROM_address_MSB_storedAutoUPshift_EngineRPM_Setpoint23 33 // Define EEPROM Address 
#define EEPROM_address_LSB_storedAutoUPshift_EngineRPM_Setpoint34 34 // Define EEPROM Address 
#define EEPROM_address_MSB_storedAutoUPshift_EngineRPM_Setpoint34 35 // Define EEPROM Address 
#define EEPROM_address_LSB_storedAutoUPshift_EngineRPM_Setpoint45 36 // Define EEPROM Address 
#define EEPROM_address_MSB_storedAutoUPshift_EngineRPM_Setpoint45 37 // Define EEPROM Address 
#define EEPROM_address_LSB_storedAutoUPshift_EngineRPM_Setpoint56 38 // Define EEPROM Address 
#define EEPROM_address_MSB_storedAutoUPshift_EngineRPM_Setpoint56 39 // Define EEPROM Address 
#define EEPROM_address_LSB_storedAutoUPshift_EngineRPM_Setpoint67 40 // Define EEPROM Address 
#define EEPROM_address_MSB_storedAutoUPshift_EngineRPM_Setpoint67 41 // Define EEPROM Address 


#define EEPROM_address_LSB_storedAutoDOWNshift_EngineRPM_Setpoint21 50  // Define EEPROM Address  
#define EEPROM_address_MSB_storedAutoDOWNshift_EngineRPM_Setpoint21 51  // Define EEPROM Address 
#define EEPROM_address_LSB_storedAutoDOWNshift_EngineRPM_Setpoint32 52  // Define EEPROM Address 
#define EEPROM_address_MSB_storedAutoDOWNshift_EngineRPM_Setpoint32 53  // Define EEPROM Address 
#define EEPROM_address_LSB_storedAutoDOWNshift_EngineRPM_Setpoint43 54  // Define EEPROM Address 
#define EEPROM_address_MSB_storedAutoDOWNshift_EngineRPM_Setpoint43 55  // Define EEPROM Address 
#define EEPROM_address_LSB_storedAutoDOWNshift_EngineRPM_Setpoint54 56  // Define EEPROM Address 
#define EEPROM_address_MSB_storedAutoDOWNshift_EngineRPM_Setpoint54 57  // Define EEPROM Address 
#define EEPROM_address_LSB_storedAutoDOWNshift_EngineRPM_Setpoint65 58  // Define EEPROM Address 
#define EEPROM_address_MSB_storedAutoDOWNshift_EngineRPM_Setpoint65 59  // Define EEPROM Address 
#define EEPROM_address_LSB_storedAutoDOWNshift_EngineRPM_Setpoint76 60  // Define EEPROM Address 
#define EEPROM_address_MSB_storedAutoDOWNshift_EngineRPM_Setpoint76 61  // Define EEPROM Address 

#define EEPROM_address_LSB_storedTC_TgtSlipRate 62 // Define EEPROM Address 
#define EEPROM_address_MSB_storedTC_TgtSlipRate 63 // Define EEPROM Address 
#define EEPROM_address_LSB_storedTC_CorrectionAggression 64 // Define EEPROM Address 
#define EEPROM_address_MSB_storedTC_CorrectionAggression 65 // Define EEPROM Address 



#define EEPROM_address_LSB_storedBurnoutMode_EngineRPM_Setpoint 70
#define EEPROM_address_MSB_storedBurnoutMode_EngineRPM_Setpoint 71

#define EEPROM_address_LSB_storedLC_EngineRPM_Setpoint_Ramp 72 // Define EEPROM Address 
#define EEPROM_address_MSB_storedLC_EngineRPM_Setpoint_Ramp 73 // Define EEPROM Address 
#define EEPROM_address_LSB_storedLC_EngineRPM_Setpoint_Hold 74 // Define EEPROM Address 
#define EEPROM_address_MSB_storedLC_EngineRPM_Setpoint_Hold 75 // Define EEPROM Address 

#define EEPROM_address_LSB_storedLC_EngineRPM_TransitionRateMultiplier_Int 76 // Define EEPROM Address 
#define EEPROM_address_LSB_storedLC_EngineRPM_RAMP_Timeout 77 // Timeout for RAMP mode (this value is multiplied by 100, to create a milliseconds timeout)
#define EEPROM_address_LSB_storedLC_EngineRPM_PID_BaseOffset 78 // this is in the range of 0-20 (a value of 30 is subtracted from this to arrive at the actual offset used )
#define EEPROM_address_LSB_storedLC_EngineRPM_PID_Ki_Term_Int 79  // LC EngineRPM_PID_iTerm_Int (This is the Ki term multiplied by 100)



#define EEPROM_address_LSB_storedLC_BoostWindow_EntryThresholdPressure 80 // Define EEPROM Address 
#define EEPROM_address_MSB_storedLC_BoostWindow_EntryThresholdPressure 81 // Define EEPROM Address 
#define EEPROM_address_LSB_storedLC_BoostWindow_minMaintainPressure 82 // Define EEPROM Address 
#define EEPROM_address_MSB_storedLC_BoostWindow_minMaintainPressure 83 // Define EEPROM Address 
#define EEPROM_address_LSB_storedLC_BoostVariableSelection 84 // Define EEPROM Address 


#define EEPROM_address_LSB_storedTemp7_Type 88
#define EEPROM_address_LSB_storedTemp8_Type 89

#define EEPROM_address_LSB_storedMQB_Parachute_PreArmedStatus 90
#define EEPROM_address_LSB_storedMQB_Parachute_ArmSpeed_Setpoint 91
#define EEPROM_address_MSB_storedMQB_Parachute_ArmSpeed_Setpoint 92




#define EEPROM_address_LSB_storedMQB_ConfigurationMode_JobNumber_to_Update 93

#define EEPROM_address_LSB_storedMQB_BoostByGearMultiplier_ActivationStatus 100
#define EEPROM_address_LSB_storedMQB_BoostByGearMultiplier_Gear1 101
#define EEPROM_address_LSB_storedMQB_BoostByGearMultiplier_Gear2 102
#define EEPROM_address_LSB_storedMQB_BoostByGearMultiplier_Gear3 103
#define EEPROM_address_LSB_storedMQB_BoostByGearMultiplier_Gear4 104
#define EEPROM_address_LSB_storedMQB_BoostByGearMultiplier_Gear5 105
#define EEPROM_address_LSB_storedMQB_BoostByGearMultiplier_Gear6 106
#define EEPROM_address_LSB_storedMQB_BoostByGearMultiplier_Gear7 107




#define EEPROM_address_LSB_LS1_Output_Type 151
#define EEPROM_address_LSB_LS2_Output_Type 152
#define EEPROM_address_LSB_LS3_Output_Type 153
#define EEPROM_address_LSB_LS4_Output_Type 154
#define EEPROM_address_LSB_LS5_Output_Type 155
#define EEPROM_address_LSB_LS6_Output_Type 156
#define EEPROM_address_LSB_HS1_Output_Type 157


#define EEPROM_address_LSB_LS1_Output_ActivationTimer_Setpoint_ms 160
#define EEPROM_address_MSB_LS1_Output_ActivationTimer_Setpoint_ms 161

#define EEPROM_address_LSB_LS2_Output_ActivationTimer_Setpoint_ms 162
#define EEPROM_address_MSB_LS2_Output_ActivationTimer_Setpoint_ms 163

#define EEPROM_address_LSB_LS3_Output_ActivationTimer_Setpoint_ms 164
#define EEPROM_address_MSB_LS3_Output_ActivationTimer_Setpoint_ms 165

#define EEPROM_address_LSB_LS4_Output_ActivationTimer_Setpoint_ms 166
#define EEPROM_address_MSB_LS4_Output_ActivationTimer_Setpoint_ms 167

#define EEPROM_address_LSB_LS5_Output_ActivationTimer_Setpoint_ms 168
#define EEPROM_address_MSB_LS5_Output_ActivationTimer_Setpoint_ms 169

#define EEPROM_address_LSB_LS6_Output_ActivationTimer_Setpoint_ms 170
#define EEPROM_address_MSB_LS6_Output_ActivationTimer_Setpoint_ms 171

#define EEPROM_address_LSB_HS1_Output_ActivationTimer_Setpoint_ms 172
#define EEPROM_address_MSB_HS1_Output_ActivationTimer_Setpoint_ms 173




#define EEPROM_address_LSB_LS1_Output_PWMFreq 181
#define EEPROM_address_LSB_LS2_Output_PWMFreq 182
#define EEPROM_address_LSB_LS3_Output_PWMFreq 183
#define EEPROM_address_LSB_LS4_Output_PWMFreq 184
#define EEPROM_address_LSB_LS5_Output_PWMFreq 185
#define EEPROM_address_LSB_LS6_Output_PWMFreq 186
#define EEPROM_address_LSB_LS6_Output_PWMFreq 186
#define EEPROM_address_LSB_HS1_Output_PWMFreq 187

#define EEPROM_address_LSB_SuspLvlSensor_FL_Direct 220
#define EEPROM_address_MSB_SuspLvlSensor_FL_Direct 221
#define EEPROM_address_LSB_SuspLvlSensor_FR_Direct 222
#define EEPROM_address_MSB_SuspLvlSensor_FR_Direct 223
#define EEPROM_address_LSB_SuspLvlSensor_RL_Direct 224
#define EEPROM_address_MSB_SuspLvlSensor_RL_Direct 225
#define EEPROM_address_LSB_SuspLvlSensor_RR_Direct 226
#define EEPROM_address_MSB_SuspLvlSensor_RR_Direct 227


#define EEPROM_address_LSB_storedHaldexPWMOutput_TrimPot_AWD_GlobalMultiplier_Position_viaConfigMode_Int 400
#define EEPROM_address_LSB_storedHaldexPWMOutput_TrimPot_AWD_CoastMinDC_Adder_Position_viaConfigMode_Int 401
#define EEPROM_address_LSB_storedHaldexPWMOutput_TrimPot_AWD_BrakePressureAdder_Multiplier_viaConfigMode_Int 402
#define EEPROM_address_LSB_storedHaldexPWMOutput_TrimPot_AWD_AccelPedalGain_Multiplier_Position_viaConfigMode_Int 403
#define EEPROM_address_LSB_storedHaldexPWMOutput_TrimPot_AWD_GradientLimiter_Position_viaConfigMode_Int 404
#define EEPROM_address_LSB_storedHaldexPWMOutput_TrimPot_AWD_MaxDC_Position_viaConfigMode_Int 405
#define EEPROM_address_LSB_storedHaldexPWMOutput_TrimPot_AWD_SteeringAngleMultiplier_Position_viaConfigMode_Int 406
#define EEPROM_address_LSB_storedHaldexPWMOutput_TrimPot_AWD_Future1_Position_viaConfigMode_Int 407
#define EEPROM_address_LSB_storedHaldexPWMOutput_TrimPot_AWD_Future2_Position_viaConfigMode_Int 408

#define EEPROM_address_LSB_storedHaldexPWMOutput_TrimPot_AWD_GlobalMultiplier_Position_Int 410
#define EEPROM_address_LSB_storedHaldexPWMOutput_TrimPot_AWD_CoastMinDC_Adder_Position_Int 411
#define EEPROM_address_LSB_storedHaldexPWMOutput_TrimPot_AWD_BrakePressureAdder_Multiplier_Int 412
#define EEPROM_address_LSB_storedHaldexPWMOutput_TrimPot_AWD_AccelPedalGain_Multiplier_Position_Int 413
#define EEPROM_address_LSB_storedHaldexPWMOutput_TrimPot_AWD_GradientLimiter_Position_Int 414
#define EEPROM_address_LSB_storedHaldexPWMOutput_TrimPot_AWD_MaxDC_Position_Int 415
#define EEPROM_address_LSB_storedHaldexPWMOutput_TrimPot_AWD_SteeringAngleMultiplier_Position_Int 416
#define EEPROM_address_LSB_storedHaldexPWMOutput_TrimPot_AWD_Future1_Position_Int 417
#define EEPROM_address_LSB_storedHaldexPWMOutput_TrimPot_AWD_Future2_Position_Int 418


#define EEPROM_address_LSB_storedBoostTargetSetting_psiG_viaConfigMode_Int 450



#pragma endregion

#pragma region // MCU Internal and Onboard Functions



extern float InternalTemp;
extern int InternalCPUTemp;
extern unsigned int CPU_SpeedMhz;
extern float CPU_Temp;


//-----------------------------------  Onboard LEDs


#ifdef Board_BasicDAZA_v2024x1

#define pinLS1_Output 36 // LED de-soldered from board

#define LED_PIN_MPU_Calibration_Status_Good 8 // Green
// extern bool blinkStateMPU_Calibration_Status_Good = false;

#define LED_PIN_MPU_Calibration_Status_Bad 9 // Red
// extern bool blinkStateMPU_Calibration_Status_Bad = false;

#define LED_PIN_MPU_Calibration_Status_Reset 34 // Red
// extern bool blinkStateMPU_Calibration_Status_Reset = false;

#define LED_PIN_CANTransmit 49
// extern bool blinkStateCANTransmit = false;

#define LED_PIN_CANRecieve 54
// extern bool blinkStateCANRecieve = false;

#define LED_PIN_CANDelay 50
// extern bool blinkStateCANDelay = false;

#endif



#ifdef Board_KitchenSink_v2023x4

#define LED_PIN_MPU_Calibration_Status_Good 8 // Green
// extern bool blinkStateMPU_Calibration_Status_Good = false;

#define LED_PIN_MPU_Calibration_Status_Bad  47// Red
// extern bool blinkStateMPU_Calibration_Status_Bad = false;

#define LED_PIN_MPU_Calibration_Status_Reset 51 // Red
// extern bool blinkStateMPU_Calibration_Status_Reset = false;

#define LED_PIN_CANTransmit 50 // Green
// extern bool blinkStateCANTransmit = false;

#define LED_PIN_CANRecieve 49 // Red
// extern bool blinkStateCANRecieve = false;

#define LED_PIN_CANDelay 48 // Red
// extern bool blinkStateCANDelay = false;

#endif

#ifdef Board_KitchenSink_v2024x4

#define LED_PIN_MPU_Calibration_Status_Good 8 // Green
// extern bool blinkStateMPU_Calibration_Status_Good = false;

#define LED_PIN_MPU_Calibration_Status_Bad  47// Red
// extern bool blinkStateMPU_Calibration_Status_Bad = false;

#define LED_PIN_MPU_Calibration_Status_Reset 51 // Red
// extern bool blinkStateMPU_Calibration_Status_Reset = false;

#define LED_PIN_CANTransmit 50 // Green
// extern bool blinkStateCANTransmit = false;

#define LED_PIN_CANRecieve 49 // Red
// extern bool blinkStateCANRecieve = false;

#define LED_PIN_CANDelay 48 // Red
// extern bool blinkStateCANDelay = false;

#endif


#ifdef Board_KitchenSink_v2025x2

#define LED_PIN_MPU_Calibration_Status_Good 8 // Green
// extern bool blinkStateMPU_Calibration_Status_Good = false;

#define LED_PIN_MPU_Calibration_Status_Bad  47// Red
// extern bool blinkStateMPU_Calibration_Status_Bad = false;

#define LED_PIN_MPU_Calibration_Status_Reset 51 // Red
// extern bool blinkStateMPU_Calibration_Status_Reset = false;

#define LED_PIN_CANTransmit 50 // Green
// extern bool blinkStateCANTransmit = false;

#define LED_PIN_CANRecieve 49 // Red
// extern bool blinkStateCANRecieve = false;

#define LED_PIN_CANDelay 48 // Red
// extern bool blinkStateCANDelay = false;

#endif

#ifdef Board_KitchenSink_v2025x3

#define LED_PIN_MPU_Calibration_Status_Good 8 // Green
// extern bool blinkStateMPU_Calibration_Status_Good = false;

#define LED_PIN_MPU_Calibration_Status_Bad  47// Red
// extern bool blinkStateMPU_Calibration_Status_Bad = false;

#define LED_PIN_MPU_Calibration_Status_Reset 51 // Red
// extern bool blinkStateMPU_Calibration_Status_Reset = false;

#define LED_PIN_CANTransmit 50 // Green
// extern bool blinkStateCANTransmit = false;

#define LED_PIN_CANRecieve 49 // Red
// extern bool blinkStateCANRecieve = false;

#define LED_PIN_CANDelay 48 // Red
// extern bool blinkStateCANDelay = false;

#endif

#ifdef Board_KitchenSink_v2025x4

#define LED_PIN_MPU_Calibration_Status_Good 8 // Green
// extern bool blinkStateMPU_Calibration_Status_Good = false;

#define LED_PIN_MPU_Calibration_Status_Bad  47// Red
// extern bool blinkStateMPU_Calibration_Status_Bad = false;

#define LED_PIN_MPU_Calibration_Status_Reset 51 // Red
// extern bool blinkStateMPU_Calibration_Status_Reset = false;

#define LED_PIN_CANTransmit 50 // Green
// extern bool blinkStateCANTransmit = false;

#define LED_PIN_CANRecieve 49 // Red
// extern bool blinkStateCANRecieve = false;

#define LED_PIN_CANDelay 48 // Red
// extern bool blinkStateCANDelay = false;

#endif




#pragma endregion

#pragma region // Analog Sensor Input Global Variables (averaged, raw ADC)

// extern int offset = 102;
// extern int fullScale = 922;

extern float SenValA0;
extern float SenValA1;
extern float SenValA2;
extern float SenValA3;
extern float SenValA4;
extern float SenValA5;
extern float SenValA6;
extern float SenValA7;
extern float SenValA8;
extern float SenValA9;
extern float SenValA10;
extern float SenValA11;
extern float SenValA12;
extern float SenValA13;
extern float SenValA14;
extern float SenValA15;
extern float SenValA16;
extern float SenValA17;


extern unsigned int SensorErrorSumBIT;
extern unsigned char msbSensorErrorSumBIT;
extern unsigned char lsbSensorErrorSumBIT;



#pragma endregion

#pragma region // NTC Thermistors

extern float Thermistor1Kelvin;
extern float Thermistor1Celcius;
extern float Thermistor1Fahrenheit;

extern float Thermistor2Kelvin;
extern float Thermistor2Celcius;
extern float Thermistor2Fahrenheit;

extern int Thermistor1C;
extern int Thermistor2C;

extern float AN_NTC1_Celcius_Float;
extern int AN_NTC1_Celcius_Int;



#pragma endregion

#pragma region // EGT and Temp Sensor Variables 


   extern unsigned int EGTErrorState;
   extern unsigned int EGT0ErrorState;
   extern unsigned int EGT1ErrorState;
   extern unsigned int EGT2ErrorState;
   extern unsigned int EGT3ErrorState;
   extern unsigned int EGT4ErrorState;
   extern unsigned int EGT5ErrorState;
   extern unsigned int EGT6ErrorState;
   extern unsigned int EGT7ErrorState;
   extern unsigned int EGT8ErrorState;
   
   extern unsigned int EGT_SafetyInterventionCombined;
   extern unsigned int EGT_HotSafetyIntervention;
   extern unsigned int EGT_ColdSafetyIntervention;
   extern unsigned int EGT0SafetyIntervention;
   extern unsigned int EGT1SafetyIntervention;
   extern unsigned int EGT2SafetyIntervention;
   extern unsigned int EGT3SafetyIntervention;
   extern unsigned int EGT4SafetyIntervention;
   extern unsigned int EGT5SafetyIntervention;
   extern unsigned int EGT6SafetyIntervention;
   extern unsigned int EGT7SafetyIntervention;
   extern unsigned int EGT8SafetyIntervention;



   //K Type Thermocouple number 1
   extern float SenValEGT0; //Sensor value
   extern float SenValEGT0ref; //Sensor value from referent pin
   extern float Temp0; //Temperature
   extern float Vref0; //Referent voltage
   extern float Vout0; //Voltage after adc
   extern float TempKelvin0;
   extern float TCelcius0;
   extern float TFahrenheit0;
    

    //K Type Thermocouple number 2
   extern float SenValEGT1; //Sensor value
   extern float SenValEGT1ref; //Sensor value from referent pin
   extern float Temp1; //Temperature
   extern float Vref1; //Referent voltage
   extern float Vout1; //Voltage after adc
   extern float TempKelvin1;
   extern float TCelcius1;
   extern float TFahrenheit1;

    //K Type Thermocouple number 3
   extern float SenValEGT2; //Sensor value
   extern float SenValEGT2ref; //Sensor value from referent pin
   extern float Temp2; //Temperature
   extern float Vref2; //Referent voltage
   extern float Vout2; //Voltage after adc
   extern float TempKelvin2;
   extern float TCelcius2;
   extern float TFahrenheit2;

    //K Type Thermocouple number 4
   extern float SenValEGT3; //Sensor value
   extern float SenValEGT3ref; //Sensor value from referent pin
   extern float Temp3; //Temperature
   extern float Vref3; //Referent voltage
   extern float Vout3; //Voltage after adc
   extern float TempKelvin3;
   extern float TCelcius3;
   extern float TFahrenheit3;

    //K Type Thermocouple number 5
   extern float SenValEGT4; //Sensor value
   extern float SenValEGT4ref; //Sensor value from referent pin
   extern float Temp4; //Temperature
   extern float Vref4; //Referent voltage
   extern float Vout4; //Voltage after adc
   extern float TempKelvin4;
   extern float TCelcius4;
   extern float TFahrenheit4;

    //K Type Thermocouple number 6
   extern float SenValEGT5; //Sensor value
   extern float SenValEGT5ref; //Sensor value from referent pin
   extern float Temp5; //Temperature
   extern float Vref5; //Referent voltage
   extern float Vout5; //Voltage after adc
   extern float TempKelvin5;
   extern float TCelcius5;
   extern float TFahrenheit5;

    //K Type Thermocouple number 7
   extern float SenValEGT6; //Sensor value
   extern float SenValEGT6ref; //Sensor value from referent pin
   extern float Temp6; //Temperature
   extern float Vref6; //Referent voltage
   extern float Vout6; //Voltage after adc
   extern float TempKelvin6;
   extern float TCelcius6;
   extern float TFahrenheit6;

    //K Type Thermocouple number 8
   extern float SenValEGT7; //Sensor value
   extern float SenValEGT7ref; //Sensor value from referent pin
   extern float Temp7; //Temperature
   extern float Vref7; //Referent voltage
   extern float Vout7; //Voltage after adc
   extern float TempKelvin7;
   extern float TCelcius7;
   extern float TFahrenheit7;



   extern int ET0;
   extern int ET1;
   extern int ET2;
   extern int ET3;
   extern int ET4;
   extern int ET5;
   extern int ET6;
   extern int ET7;
   extern int EGTavg;

   extern unsigned int ET0_Int;
   extern unsigned int ET1_Int;
   extern unsigned int ET2_Int;
   extern unsigned int ET3_Int;
   extern unsigned int ET4_Int;
   extern unsigned int ET5_Int;
   extern unsigned int ET6_Int;
   extern unsigned int ET7_Int;
   extern unsigned int EGTavg_Int ;

    
   extern int EGT0deltaInt;
   extern int EGT1deltaInt;
   extern int EGT2deltaInt;
   extern int EGT3deltaInt;
   extern int EGT4deltaInt;
   extern int EGT5deltaInt;
   extern int EGT6deltaInt;
   extern int EGT7deltaInt;
    
   extern float EGT0deltaPct;
   extern float EGT1deltaPct;
   extern float EGT2deltaPct;
   extern float EGT3deltaPct;
   extern float EGT4deltaPct;
   extern float EGT5deltaPct;
   extern float EGT6deltaPct;
   extern float EGT7deltaPct;

   extern float EGT_DeltaTolerancePct;
   extern int EGT_DeltaToleranceInt;

  extern unsigned char lsbEGT0, msbEGT0, lsbEGT1, msbEGT1, lsbEGT2, msbEGT2, lsbEGT3, msbEGT3, lsbEGT4, msbEGT4, lsbEGT5, msbEGT5, lsbEGT6, msbEGT6, lsbEGT7, msbEGT7, lsbEGTavg, msbEGTavg;


#pragma endregion

#pragma region // declaring averaging variables

#define WINDOW_SIZE 32
// #define WINDOW_SIZE_AccelXY 192
#define WINDOW_SIZE_EGTs 256
// #define WINDOW_SIZE_AccelZ 256


extern unsigned int INDEXA0;
extern unsigned int VALUEA0;
extern unsigned int SUMA0;
extern unsigned int READINGSA0[WINDOW_SIZE_EGTs];
extern unsigned int AVERAGEDA0;

extern unsigned int INDEXA1;
extern unsigned int VALUEA1;
extern unsigned int SUMA1;
extern unsigned int READINGSA1[WINDOW_SIZE_EGTs];
extern float AVERAGEDA1;

extern unsigned int INDEXA2;
extern unsigned int VALUEA2;
extern unsigned int SUMA2;
extern unsigned int READINGSA2[WINDOW_SIZE_EGTs];
extern unsigned int AVERAGEDA2;

extern unsigned int INDEXA3;
extern unsigned int VALUEA3;
extern unsigned int SUMA3;
extern unsigned int READINGSA3[WINDOW_SIZE_EGTs];
extern unsigned int AVERAGEDA3;

extern unsigned int INDEXA4;
extern unsigned int VALUEA4;
extern unsigned int SUMA4;
extern unsigned int READINGSA4[WINDOW_SIZE];
extern unsigned int AVERAGEDA4;
 
extern unsigned int INDEXA5;
extern unsigned int VALUEA5;
extern unsigned int SUMA5;
extern unsigned int READINGSA5[WINDOW_SIZE];
extern unsigned int AVERAGEDA5;
 
extern unsigned int INDEXA6;
extern unsigned int VALUEA6;
extern unsigned int SUMA6;
extern unsigned int READINGSA6[WINDOW_SIZE_EGTs];
extern unsigned int AVERAGEDA6;
 
extern unsigned int INDEXA7;
extern unsigned int VALUEA7;
extern unsigned int SUMA7;
extern unsigned int READINGSA7[WINDOW_SIZE_EGTs];
extern unsigned int AVERAGEDA7;

extern unsigned int INDEXA8;
extern unsigned int VALUEA8;
extern unsigned int SUMA8;
extern unsigned int READINGSA8[WINDOW_SIZE];
extern unsigned int AVERAGEDA8;
 
extern unsigned int INDEXA9;
extern unsigned int VALUEA9;
extern unsigned int SUMA9;
extern unsigned int READINGSA9[WINDOW_SIZE];
extern unsigned int AVERAGEDA9;
 
extern unsigned int INDEXA10;
extern unsigned int VALUEA10;
extern unsigned int SUMA10;
extern unsigned int READINGSA10[WINDOW_SIZE_EGTs];
extern unsigned int AVERAGEDA10;

extern unsigned int INDEXA11;
extern unsigned int VALUEA11;
extern unsigned int SUMA11;
extern unsigned int READINGSA11[WINDOW_SIZE_EGTs];
extern unsigned int AVERAGEDA11;

extern unsigned int INDEXA12;
extern unsigned int VALUEA12;
extern unsigned int SUMA12;
extern unsigned int READINGSA12[WINDOW_SIZE_EGTs];
extern unsigned int AVERAGEDA12;

extern unsigned int INDEXA13;
extern unsigned int VALUEA13;
extern unsigned int SUMA13;
extern unsigned int READINGSA13[WINDOW_SIZE_EGTs];
extern unsigned int AVERAGEDA13;

extern unsigned int INDEXA14;
extern unsigned int VALUEA14;
extern unsigned int SUMA14;
extern unsigned int READINGSA14[WINDOW_SIZE];
extern unsigned int AVERAGEDA14;

extern unsigned int INDEXA15;
extern unsigned int VALUEA15;
extern unsigned int SUMA15;
extern unsigned int READINGSA15[WINDOW_SIZE];
extern unsigned int AVERAGEDA15;
 
extern unsigned int INDEXA16;
extern unsigned int VALUEA16;
extern unsigned int SUMA16;
extern unsigned int READINGSA16[WINDOW_SIZE];
extern unsigned int AVERAGEDA16;
 
extern unsigned int INDEXA17;
extern unsigned int VALUEA17;
extern unsigned int SUMA17;
extern unsigned int READINGSA17[WINDOW_SIZE];
extern unsigned int AVERAGEDA17;

extern float filteredA0;
extern float filteredA1;
extern float filteredA2;
extern float filteredA3;
extern float filteredA4;
extern float filteredA5;
extern float filteredA6;
extern float filteredA7;
extern float filteredA8;
extern float filteredA9;
extern float filteredA10;
extern float filteredA11;
extern float filteredA12;
extern float filteredA13;
extern float filteredA14;
extern float filteredA15;
extern float filteredA16;
extern float filteredA17;
    
extern unsigned int filteredA0_Int;
extern unsigned int filteredA1_Int;
extern unsigned int filteredA2_Int;
extern unsigned int filteredA3_Int;
extern unsigned int filteredA4_Int;
extern unsigned int filteredA5_Int;
extern unsigned int filteredA6_Int;
extern unsigned int filteredA7_Int;
extern unsigned int filteredA8_Int;
extern unsigned int filteredA9_Int;
extern unsigned int filteredA10_Int;
extern unsigned int filteredA11_Int;
extern unsigned int filteredA12_Int;
extern unsigned int filteredA13_Int;
extern unsigned int filteredA14_Int;
extern unsigned int filteredA15_Int;
extern unsigned int filteredA16_Int;
extern unsigned int filteredA17_Int;


// EWMA Variables

#pragma endregion

#pragma region // PT_CAN_Calculated Pressure Values 

extern float PressureA0v;
extern float PressureA1v;
extern float PressureA2v;
extern float PressureA3v;
extern float PressureA8v;
extern float PressureA9v;
extern float PressureA10v;
extern float PressureA11v;
extern float PressureA12v;
extern float PressureA13v;
extern float PressureA14v;
extern float PressureA15v;
extern float PressureA16v;
extern float PressureA17v;

extern unsigned int PressureA0v_Int;
extern unsigned int PressureA1v_Int;
extern unsigned int PressureA2v_Int;
extern unsigned int PressureA3v_Int;
extern unsigned int PressureA8v_Int;
extern unsigned int PressureA9v_Int;
extern unsigned int PressureA10v_Int;
extern unsigned int PressureA11v_Int;
extern unsigned int PressureA12v_Int;
extern unsigned int PressureA13v_Int;
extern unsigned int PressureA14v_Int;
extern unsigned int PressureA15v_Int;
extern unsigned int PressureA16v_Int;
extern unsigned int PressureA17v_Int;


extern unsigned int PressureA0;
extern unsigned int PressureA1;
extern unsigned int PressureA2;
extern unsigned int PressureA3;
extern unsigned int PressureA8;
extern unsigned int PressureA9;
extern unsigned int PressureA10;
extern unsigned int PressureA11;
extern unsigned int PressureA12;
extern unsigned int PressureA13;
extern unsigned int PressureA14;
extern unsigned int PressureA15;
extern unsigned int PressureA16;
extern unsigned int PressureA17;

#pragma endregion

#pragma region // Digital/Freq Sensor Inputs


extern unsigned long int turboshaftRPM1;
extern unsigned long int turboshaftRPM2;
extern unsigned long int turboshaftRPM1corrected;
extern unsigned long int turboshaftRPM1raw;

extern volatile int compressorblade_pulse;

extern float Frequency1;
extern float Frequency2;
extern float Frequency3;
 
extern float period1;
extern float DutyCycle1;
extern float period2;
extern float DutyCycle2;
extern float period3;
extern float DutyCycle3;



#pragma endregion

#pragma region // External Sensor Error Reporting

extern unsigned int SensorErrorA0; 
extern unsigned int SensorErrorA1; 
extern unsigned int SensorErrorA2; 
extern unsigned int SensorErrorA3; 
extern unsigned int SensorErrorA8; 
extern unsigned int SensorErrorA9; 
extern unsigned int SensorErrorA10; 
extern unsigned int SensorErrorA11; 
extern unsigned int SensorErrorA12; 
extern unsigned int SensorErrorA13; 
extern unsigned int SensorErrorA14; 
extern unsigned int SensorErrorA15; 
extern unsigned int SensorErrorA16; 
extern unsigned int SensorErrorA17; 


extern unsigned int SensorErrorA0bit; 
extern unsigned int SensorErrorA1bit; 
extern unsigned int SensorErrorA2bit; 
extern unsigned int SensorErrorA3bit; 
extern unsigned int SensorErrorA8bit; 
extern unsigned int SensorErrorA9bit; 
extern unsigned int SensorErrorA10bit; 
extern unsigned int SensorErrorA11bit; 
extern unsigned int SensorErrorA12bit; 
extern unsigned int SensorErrorA13bit; 
extern unsigned int SensorErrorA14bit; 
extern unsigned int SensorErrorA15bit; 
extern unsigned int SensorErrorA16bit; 
extern unsigned int SensorErrorA17bit; 

extern unsigned int SensorErrorSumBIT;



#pragma endregion

#pragma region // TFT GPIO Output Variables

    #ifdef Board_BasicDAZA_v2024x1
      #define pinLS1_Output 36
      #endif

    #ifdef Board_KitchenSink_v2023x4
      #define pinLS1_Output 36
      #define pinLS2_Output 37
      #define pinLS3_Output 2
      #define pinLS4_Output 33
      #define pinLS5_Output 12
      #define pinLS6_Output 1
      #define pinLS7_Output 8
      #define pinLS8_Output 8
      #define pinHS1_Output 34

      #define pin_TSS 5

      #endif

    #ifdef Board_KitchenSink_v2024x1
      #define pinLS1_Output 36
      #define pinLS2_Output 37
      #define pinLS3_Output 2
      #define pinLS4_Output 33
      #define pinLS5_Output 12
      #define pinLS6_Output 1
      #define pinLS7_Output 8
      #define pinLS8_Output 8
      #define pinHS1_Output 34

      #define pin_TSS 5

      #endif


    #ifdef Board_KitchenSink_v2024x4
      #define pinLS1_Output 36
      #define pinLS2_Output 37
      #define pinLS3_Output 2
      #define pinLS4_Output 33
      #define pinLS5_Output 12
      #define pinLS6_Output 1
      #define pinLS7_Output 8
      #define pinLS8_Output 8
      #define pinHS1_Output 34

      #define pin_TSS 5

      #endif
      
    #ifdef Board_KitchenSink_v2025x2
      #define pinLS1_Output 36
      #define pinLS2_Output 37
      #define pinLS3_Output 2
      #define pinLS4_Output 33
      #define pinLS5_Output 12
      #define pinLS6_Output 1
      #define pinLS7_Output 8
      #define pinLS8_Output 8
      #define pinHS1_Output 34

      #define pin_TSS 5

      #endif

    #ifdef Board_KitchenSink_v2025x3
      #define pinLS1_Output 36
      #define pinLS2_Output 37
      #define pinLS3_Output 2
      #define pinLS4_Output 33
      #define pinLS5_Output 12
      #define pinLS6_Output 1
      // #define pinLS7_Output 8
      // #define pinLS8_Output 8
      #define pinHS1_Output 34
      #define pinHS2_Output 53

      #define pin_TSS 5

      #endif


      
    #ifdef Board_DAZA_DI_Delete_v1x2
      #define pinLS1_Output 1
      #define pinLS2_Output 2
      #define pinLS3_Output 3
      #define pinLS4_Output 4
      #define pinLS5_Output 5
      #define pinLS6_Output 6
      #define pinLS7_Output 36
      #define pinLS8_Output 37
      #define pinHS1_Output 34

      #endif





      extern unsigned int LS1_Signalvia0x743_Int;
      extern unsigned int LS2_Signalvia0x743_Int;
      extern unsigned int LS3_Signalvia0x743_Int;
      extern unsigned int LS4_Signalvia0x743_Int;
      extern unsigned int LS5_Signalvia0x743_Int;
      extern unsigned int LS6_Signalvia0x743_Int;
      extern unsigned int HS1_Signalvia0x743_Int;
      extern unsigned int HS2_Signalvia0x743_Int;
       
      extern unsigned int LS1_OutputStatus;
      extern unsigned int LS2_OutputStatus;
      extern unsigned int LS3_OutputStatus;
      extern unsigned int LS4_OutputStatus;
      extern unsigned int LS5_OutputStatus;
      extern unsigned int LS6_OutputStatus;
      extern unsigned int HS1_OutputStatus;
      extern unsigned int HS2_OutputStatus;

      extern unsigned int LS1_Output_Status;
      extern unsigned int HS1_Output_Status;
      extern unsigned int HS2_Output_Status;
      extern unsigned int LPFP_TriggerActive;
  
 
      extern unsigned int LS1_Output_Digital;
      extern unsigned int LS2_Output_Digital;
      extern unsigned int LS3_Output_Digital;
      extern unsigned int LS4_Output_Digital;
      extern unsigned int LS5_Output_Digital;
      extern unsigned int LS6_Output_Digital;
      extern unsigned int HS1_Output_Digital;
      extern unsigned int HS2_Output_Digital;
 
      extern unsigned int LS1_Output_PWMDC;
      extern unsigned int LS2_Output_PWMDC;
      extern unsigned int LS3_Output_PWMDC;
      extern unsigned int LS4_Output_PWMDC;
      extern unsigned int LS5_Output_PWMDC;
      extern unsigned int LS6_Output_PWMDC;
      extern unsigned int HS1_Output_PWMDC;
      extern unsigned int HS2_Output_PWMDC;
 
      extern unsigned int LS1_Output_PWMDC_mapped;
      extern unsigned int LS2_Output_PWMDC_mapped;
      extern unsigned int LS3_Output_PWMDC_mapped;
      extern unsigned int LS4_Output_PWMDC_mapped;
      extern unsigned int LS5_Output_PWMDC_mapped;
      extern unsigned int LS6_Output_PWMDC_mapped;
      extern unsigned int HS1_Output_PWMDC_mapped;
      extern unsigned int HS2_Output_PWMDC_mapped;
 
      extern unsigned int LS1_Output_ActivationTimer_Setpoint_ms;
      extern unsigned int LS2_Output_ActivationTimer_Setpoint_ms;
      extern unsigned int LS3_Output_ActivationTimer_Setpoint_ms;
      extern unsigned int LS4_Output_ActivationTimer_Setpoint_ms;
      extern unsigned int LS5_Output_ActivationTimer_Setpoint_ms;
      extern unsigned int LS6_Output_ActivationTimer_Setpoint_ms;
      extern unsigned int HS1_Output_ActivationTimer_Setpoint_ms;
      extern unsigned int HS2_Output_ActivationTimer_Setpoint_ms;
 
      extern unsigned int LS1_Output_ActivationTimer_Setpoint_Mode3_ms;
      extern unsigned int LS2_Output_ActivationTimer_Setpoint_Mode3_ms;
      extern unsigned int LS3_Output_ActivationTimer_Setpoint_Mode3_ms;
      extern unsigned int LS4_Output_ActivationTimer_Setpoint_Mode3_ms;
      extern unsigned int LS5_Output_ActivationTimer_Setpoint_Mode3_ms;
      extern unsigned int LS6_Output_ActivationTimer_Setpoint_Mode3_ms;
      extern unsigned int HS1_Output_ActivationTimer_Setpoint_Mode3_ms;
      extern unsigned int HS2_Output_ActivationTimer_Setpoint_Mode3_ms;


      extern elapsedMillis LS1_Output_ActivationTimer;
      extern elapsedMillis LS2_Output_ActivationTimer;
      extern elapsedMillis LS3_Output_ActivationTimer;
      extern elapsedMillis LS4_Output_ActivationTimer;
      extern elapsedMillis LS5_Output_ActivationTimer;
      extern elapsedMillis LS6_Output_ActivationTimer;
      extern elapsedMillis HS1_Output_ActivationTimer;
      extern elapsedMillis HS2_Output_ActivationTimer;


      extern unsigned int SurgeTank_Pump1_RelayTrigger_PWMDC;
      extern unsigned int SurgeTank_Pump2_RelayTrigger_PWMDC;
      extern unsigned int SurgeTank_Pump3_RelayTrigger_PWMDC;
      extern unsigned int InTank_SiphonPump1_RelayTrigger_PWMDC;

      extern unsigned int SurgeTank_Pump1_RelayTrigger_PWMDC_Mapped;
      extern unsigned int SurgeTank_Pump2_RelayTrigger_PWMDC_Mapped;
      extern unsigned int SurgeTank_Pump3_RelayTrigger_PWMDC_Mapped;
      extern unsigned int InTank_SiphonPump1_RelayTrigger_PWMDC_Mapped;

// Standalone HaldexController Variables ---------------------------------------------------------------------------------------------------------------------------------


#pragma region  // Declarations of Haldex Variables

// ---------------- Trim Pots and Switches
   extern float tempVSS_A0;
   extern int tempAccelPedal_A1;
   extern float tempBrakePres_A2;
   extern float tempFrontSlipPct;
   extern float tempRearSlipPct;
   extern float tempCenterSlipPct;


      // -------------------------------------------------------------------------------------------------

      extern float HaldexPWMOutput_TrimPot_AWD_GlobalMultiplier_Position;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_GlobalMultiplier_Position_Raw;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_GlobalMultiplier_Position_Int;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_GlobalMultiplier_Position_viaConfigMode_Int;

      extern float HaldexPWMOutput_TrimPot_AWD_GlobalMultiplier;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_GlobalMultiplier_Int;

      // -------------------------------------------------------------------------------------------------

      extern float HaldexPWMOutput_TrimPot_AWD_CoastMinDC_Adder_Position;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_CoastMinDC_Adder_Position_Raw;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_CoastMinDC_Adder_Position_Int;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_CoastMinDC_Adder_Position_viaConfigMode_Int;

      extern float HaldexPWMOutput_TrimPot_AWD_CoastMinDC_Adder;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_CoastMinDC_Adder_Int;

      // -------------------------------------------------------------------------------------------------

      extern float HaldexPWMOutput_TrimPot_AWD_BrakePressureAdder_Multiplier_Position;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_BrakePressureAdder_Multiplier_Position_Raw;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_BrakePressureAdder_Multiplier_Position_Int;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_BrakePressureAdder_Multiplier_Position_viaConfigMode_Int;

      extern float HaldexPWMOutput_TrimPot_AWD_BrakePressureAdder_Multiplier;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_BrakePressureAdder_Multiplier_Int;

      // -------------------------------------------------------------------------------------------------
      
      extern float HaldexPWMOutput_TrimPot_AWD_AccelPedalGain_Multiplier_Position;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_AccelPedalGain_Multiplier_Position_Raw;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_AccelPedalGain_Multiplier_Position_Int;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_AccelPedalGain_Multiplier_Position_viaConfigMode_Int;

      extern float HaldexPWMOutput_TrimPot_AWD_AccelPedalGain_Multiplier;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_GlobalMultiplier_Int;

      // -------------------------------------------------------------------------------------------------

      extern float HaldexPWMOutput_TrimPot_AWD_GradientLimiter_Position;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_GradientLimiter_Position_Raw;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_GradientLimiter_Position_Int;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_GradientLimiter_Position_viaConfigMode_Int;

      extern float HaldexPWMOutput_TrimPot_AWD_GradientLimiter_Float;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_GradientLimiter_Int;

      // -------------------------------------------------------------------------------------------------

      extern float HaldexPWMOutput_TrimPot_AWD_MaxDC_Position;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_MaxDC_Position_Raw;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_MaxDC_Position_Int;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_MaxDC_Position_viaConfigMode_Int;

      extern float HaldexPWMOutput_TrimPot_AWD_MaxDC_Float;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_MaxDC_Int;

      // -------------------------------------------------------------------------------------------------

      extern float HaldexPWMOutput_TrimPot_AWD_SteeringAngleMultiplier_Position;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_SteeringAngleMultiplier_Position_Raw;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_SteeringAngleMultiplier_Position_Int;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_SteeringAngleMultiplier_Position_viaConfigMode_Int;

      extern float HaldexPWMOutput_TrimPot_AWD_SteeringAngleMultiplier;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_SteeringAngleMultiplier_Int;

      // -------------------------------------------------------------------------------------------------

      extern float HaldexPWMOutput_TrimPot_AWD_Future1_Position;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_Future1_Position_Raw;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_Future1_Position_Int;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_Future1_viaConfigMode_Int;

      extern float HaldexPWMOutput_TrimPot_AWD_Future1_Float;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_Future1_Int;

      // -------------------------------------------------------------------------------------------------
      
      extern float HaldexPWMOutput_TrimPot_AWD_Future1_Position;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_Future2_Position_Raw;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_Future2_Position_Int;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_Future2_viaConfigMode_Int;

      extern float HaldexPWMOutput_TrimPot_AWD_Future2_Float;
      extern unsigned int HaldexPWMOutput_TrimPot_AWD_Future2_Int;


      // -------------------------------------------------------------------------------------------------



   extern float HaldexPWMOutput_Base_Float; // This is the result of AccelPedal_0x391_Mapped_Float multliplied by HaldexPWMOutput_TrimPot_AWD_AccelPedalGain_Multiplier, which is constrained by the maximum DC
   extern unsigned int HaldexPWMOutput_Base_Int; // This is the result of AccelPedal_0x391_Mapped_Float multliplied by HaldexPWMOutput_TrimPot_AWD_AccelPedalGain_Multiplier, which is constrained by the maximum DC
   extern float HaldexPWMOutput_BaseAfterAccelPedalMultiplier_Float;
   extern unsigned int HaldexPWMOutput_BaseAfterAccelPedalMultiplier_Int;

   extern float HaldexPWMOutput_FrontSlip_Adder_Float;
   extern unsigned int HaldexPWMOutput_FrontSlip_Adder_Int;
   extern float HaldexPWMOutput_RearSlip_Adder_Float;
   extern unsigned int HaldexPWMOutput_RearSlip_Adder_Int;
   extern float HaldexPWMOutput_CenterSlip_Adder_Float;
   extern unsigned int HaldexPWMOutput_CenterSlip_Adder_Int;
   extern float HaldexPWMOutput_SumofAllAdders_Float;
   extern unsigned int HaldexPWMOutput_SumofAllAdders_Int;
   extern float HaldexPWM_Output_AfterAllAdders_Float;
   extern unsigned int HaldexPWM_Output_AfterAllAdders_Int;

   extern float HaldexPWMOutput_BrakePressureAdder_Float;
   extern unsigned int HaldexPWMOutput_BrakePressureAdder_Int;

   extern float HaldexPWMOutput_SteeringAngleMultiplier_Float;
   extern unsigned int HaldexPWMOutput_SteeringAngleMultiplier_Int;

   extern float HaldexPWMOutput_AWDPot_Multiplier_Float;
   extern float HaldexPWMOutput_CoastMinDC_Adder_Float;
   extern unsigned int HaldexPWMOutput_CoastMinDC_Adder_Int;

   extern float HaldexPWMOutput_AfterSWA_Multiplier_Float;
   extern unsigned int HaldexPWM_Output_AfterSWA_Multiplier_Int;

   extern float HaldexPWM_Output_AfterAllMultipliers_Float;
   extern unsigned int HaldexPWM_Output_AfterAllMultipliers_Int;

   extern float HaldexPWM_OutputFinal_Float;
   extern unsigned int HaldexPWMOutput_Final_Int;
   extern unsigned int HaldexPWMOutput_Final_Int_8bitmapped;





#pragma endregion


  extern float ASV_BOVControl_DutyCycle_Float;
  extern unsigned int ASV_BOVControl_TargetDomePressure_Int;
  extern unsigned int ASV_BOVControl_DutyCycle_Int;
  extern unsigned int ASV_BOVControl_DutyCycle_Int_8bitmapped;


// Configurations Setup ---------------------------------------------------------------------------------------------------------------------------------

      extern unsigned int LS1_Output_Configuration_Mode_Active;
      extern unsigned int LS2_Output_Configuration_Mode_Active;
      extern unsigned int LS3_Output_Configuration_Mode_Active;
      extern unsigned int LS4_Output_Configuration_Mode_Active;
      extern unsigned int LS5_Output_Configuration_Mode_Active;
      extern unsigned int LS6_Output_Configuration_Mode_Active;
      extern unsigned int HS1_Output_Configuration_Mode_Active;
      extern unsigned int HS2_Output_Configuration_Mode_Active;
      
      extern unsigned int LS1_Output_Type;
      extern unsigned int LS2_Output_Type;
      extern unsigned int LS3_Output_Type;
      extern unsigned int LS4_Output_Type;
      extern unsigned int LS5_Output_Type;
      extern unsigned int LS6_Output_Type;
      extern unsigned int HS1_Output_Type;
      extern unsigned int HS2_Output_Type;

      extern unsigned int LS1_Output_ActivationTimer_UpdateMode_Active;
      extern unsigned int LS2_Output_ActivationTimer_UpdateMode_Active;
      extern unsigned int LS3_Output_ActivationTimer_UpdateMode_Active;
      extern unsigned int LS4_Output_ActivationTimer_UpdateMode_Active;
      extern unsigned int LS5_Output_ActivationTimer_UpdateMode_Active;
      extern unsigned int LS6_Output_ActivationTimer_UpdateMode_Active;
      extern unsigned int HS1_Output_ActivationTimer_UpdateMode_Active;
      extern unsigned int HS2_Output_ActivationTimer_UpdateMode_Active;

      extern unsigned int LS1_Output_ActivationTimer_Setpoint_Mode3_ms;
      extern unsigned int LS2_Output_ActivationTimer_Setpoint_Mode3_ms;
      extern unsigned int LS3_Output_ActivationTimer_Setpoint_Mode3_ms;
      extern unsigned int LS4_Output_ActivationTimer_Setpoint_Mode3_ms;
      extern unsigned int LS5_Output_ActivationTimer_Setpoint_Mode3_ms;
      extern unsigned int LS6_Output_ActivationTimer_Setpoint_Mode3_ms;
      extern unsigned int HS1_Output_ActivationTimer_Setpoint_Mode3_ms;
      extern unsigned int HS2_Output_ActivationTimer_Setpoint_Mode3_ms;

      extern unsigned int LS1_Output_ActivationTimer_Setpoint_ms;
      extern unsigned int LS2_Output_ActivationTimer_Setpoint_ms;
      extern unsigned int LS3_Output_ActivationTimer_Setpoint_ms;
      extern unsigned int LS4_Output_ActivationTimer_Setpoint_ms;
      extern unsigned int LS5_Output_ActivationTimer_Setpoint_ms;
      extern unsigned int LS6_Output_ActivationTimer_Setpoint_ms;
      extern unsigned int HS1_Output_ActivationTimer_Setpoint_ms;
      extern unsigned int HS2_Output_ActivationTimer_Setpoint_ms;

      extern unsigned int LS1_Output_SwitchTimerDisplay;
      extern unsigned int LS2_Output_SwitchTimerDisplay;
      extern unsigned int LS3_Output_SwitchTimerDisplay;
      extern unsigned int LS4_Output_SwitchTimerDisplay;
      extern unsigned int LS5_Output_SwitchTimerDisplay;
      extern unsigned int LS6_Output_SwitchTimerDisplay;
      extern unsigned int HS1_Output_SwitchTimerDisplay;
      extern unsigned int HS2_Output_SwitchTimerDisplay;

      extern unsigned int LS1_Output_PWMFreq_UpdateMode_Active;
      extern unsigned int LS2_Output_PWMFreq_UpdateMode_Active;
      extern unsigned int LS3_Output_PWMFreq_UpdateMode_Active;
      extern unsigned int LS4_Output_PWMFreq_UpdateMode_Active;
      extern unsigned int LS5_Output_PWMFreq_UpdateMode_Active;
      extern unsigned int LS6_Output_PWMFreq_UpdateMode_Active;
      extern unsigned int HS1_Output_PWMFreq_UpdateMode_Active;
      extern unsigned int HS2_Output_PWMFreq_UpdateMode_Active;

      extern unsigned int LS1_Output_PWMFreq;
      extern unsigned int LS2_Output_PWMFreq;
      extern unsigned int LS3_Output_PWMFreq;
      extern unsigned int LS4_Output_PWMFreq;
      extern unsigned int LS5_Output_PWMFreq;
      extern unsigned int LS6_Output_PWMFreq;
      extern unsigned int HS1_Output_PWMFreq;
      extern unsigned int HS2_Output_PWMFreq;


#pragma endregion

#pragma region // TFT LC EngineRPM PID Controls


extern float pTerm;
extern float pTerm2;
extern float pTerm3;
extern signed int Output_8bitSigned;
extern double pTermVal;
extern double iTermVal;
extern double dTermVal;

extern int LC_EngineRPM_SetpointPID;

extern signed int LC_IgnOffsetBase;



#pragma endregion

#pragma region // IMU Variables for global use

// Yaw, Pitch, Roll
extern int yaw;
extern int pitch;
extern int roll;

extern int yaw2;
extern int pitch2;
extern int roll2;

// Real Acceleration XYZ
extern int accelX;
extern int accelY;
extern int accelZ;



extern int VALUEAccelXAvg;
extern int AVERAGEDAccelXAvg;
extern int SenValAccelXAvg;

extern int VALUEAccelYAvg;
extern int AVERAGEDAccelYAvg;
extern int SenValAccelYAvg;

extern int VALUEAccelZAvg;
extern int AVERAGEDAccelZAvg;
extern int SenValAccelZAvg;


extern int shortAccelX;
extern int shortAccelY;
extern int shortAccelZ;

extern int shiftedAccelXms;
extern int shiftedAccelYms;
extern int shiftedAccelZms;


extern int SUMAccelXAvg;
extern int SUMAccelYAvg;
extern int SUMAccelZAvg;

extern int mappedAccelX;
extern int mappedAccelY;
extern int mappedAccelZ;

extern int mappedAccelXms;
extern int mappedAccelYms;
extern int mappedAccelZms;


extern signed int OffsetAccelX;
extern signed int OffsetAccelY;
extern signed int OffsetAccelZ;

extern signed int OffsetGyroX;
extern signed int OffsetGyroY;
extern signed int OffsetGyroZ;



extern bool IMUCalibration_HasRun;
extern unsigned int IMUCalibration_BrakePedalAcceptableCondition;
extern unsigned int IMUCalibration_EngRPMAcceptableCondition;
extern unsigned int IMUCalibration_GearLeverPositionAcceptableCondition;
extern unsigned int IMUCalibration_ParkBrakeStatusAcceptableCondition;
extern unsigned int IMUCalibration_CruiseStalkAcceptableCondition;
extern unsigned int IMUCalibration_Trigger;
extern unsigned int IMUCalibration_AcceptableConditions;
extern unsigned int IMUCalibration_Running;
extern unsigned int IMU_Calibration_Mode;
extern unsigned int IMU_Status;


#pragma endregion

#pragma region // Suspension Level Sensors - OBD

extern unsigned int InstructionFromMaster_Susp_LvlSnsr_Offsets_Reset;
extern bool Susp_LvlSnsr_Direct_Offset_Reset_HasRun;
extern bool Susp_LvlSnsr_Offset_Reset_HasRun;
extern unsigned int Susp_LvlSnsr_Offset_Reset_AcceptableConditions;


extern unsigned int MQB_SRI_EngineOil_Reset_Mode;
extern unsigned int MQB_SRI_TransOil_Reset_Mode;

extern signed int Offset_LevelSensor_FL;
extern signed int Offset_LevelSensor_FR;
extern signed int Offset_LevelSensor_RL;
extern signed int Offset_LevelSensor_RR;

extern int OBD_CAN_FL_Height;
extern int OBD_CAN_FR_Height;
extern int OBD_CAN_RL_Height;
extern int OBD_CAN_RR_Height;

extern int rawLevelSensor_FL;
extern int rawLevelSensor_FR;
extern int rawLevelSensor_RL;
extern int rawLevelSensor_RR;

extern int newLevelSensor_FL;
extern int newLevelSensor_FR;
extern int newLevelSensor_RL;
extern int newLevelSensor_RR;

extern unsigned int UnsignedLevelSensor_FL;
extern unsigned int UnsignedLevelSensor_FR;
extern unsigned int UnsignedLevelSensor_RL;
extern unsigned int UnsignedLevelSensor_RR;


// ** declared below in EEPROM stored data 

// extern signed int storedOffset_LevelSensor_FL;
// extern signed int storedOffset_LevelSensor_FR;
// extern signed int storedOffset_LevelSensor_RL;
// extern signed int storedOffset_LevelSensor_RR;

// extern signed int initialOffset_LevelSensor_FL;
// extern signed int initialOffset_LevelSensor_FR;
// extern signed int initialOffset_LevelSensor_RL;
// extern signed int initialOffset_LevelSensor_RR;

extern unsigned char lsbLevelSensor_FL, msbLevelSensor_FL, lsbLevelSensor_FR, msbLevelSensor_FR, lsbLevelSensor_RL, msbLevelSensor_RL, lsbLevelSensor_RR, msbLevelSensor_RR;


#pragma endregion

#pragma region // Digital/Freq Sensor Inputs - Suspension Level Sensors Direct

  //Suspension
  extern signed int Direct_FL_Height; // Front Left Ride Height (Frequency value including offset)
  extern signed int Direct_FR_Height; // Front Right Ride Height (Frequency value including offset)
  extern signed int Direct_RL_Height; // Rear Left Ride Height (Frequency value including offset)
  extern signed int Direct_RR_Height; // Rear Right Ride Height (Frequency value including offset)

  extern signed int Suspension_Offset_LevelSensor_FL_Direct;
  extern signed int Suspension_Offset_LevelSensor_FR_Direct;
  extern signed int Suspension_Offset_LevelSensor_RL_Direct;
  extern signed int Suspension_Offset_LevelSensor_RR_Direct;


  extern unsigned int EEPROM_val_LSB_SuspLvlSensor_FL_Direct; // Suspension Level Sensor_FL_LSB
  extern unsigned int EEPROM_val_MSB_SuspLvlSensor_FL_Direct; // Suspension Level Sensor_FL_LSB
  extern unsigned int EEPROM_val_LSB_SuspLvlSensor_FR_Direct; // Suspension Level Sensor_FR_LSB
  extern unsigned int EEPROM_val_MSB_SuspLvlSensor_FR_Direct; // Suspension Level Sensor_FR_LSB
  extern unsigned int EEPROM_val_LSB_SuspLvlSensor_RL_Direct; // Suspension Level Sensor_RL_LSB
  extern unsigned int EEPROM_val_MSB_SuspLvlSensor_RL_Direct; // Suspension Level Sensor_RL_LSB
  extern unsigned int EEPROM_val_LSB_SuspLvlSensor_RR_Direct; // Suspension Level Sensor_RR_LSB
  extern unsigned int EEPROM_val_MSB_SuspLvlSensor_RR_Direct; // Suspension Level Sensor_RR_LSB    
  

  extern signed int storedOffset_LevelSensor_FL_Direct;
  extern signed int storedOffset_LevelSensor_FR_Direct;
  extern signed int storedOffset_LevelSensor_RL_Direct;
  extern signed int storedOffset_LevelSensor_RR_Direct;


  extern signed int initialOffset_LevelSensor_FL_Direct;
  extern signed int initialOffset_LevelSensor_FR_Direct;
  extern signed int initialOffset_LevelSensor_RL_Direct;
  extern signed int initialOffset_LevelSensor_RR_Direct;

  extern signed int Offset_LevelSensor_FL_Direct;
  extern signed int Offset_LevelSensor_FR_Direct;
  extern signed int Offset_LevelSensor_RL_Direct;
  extern signed int Offset_LevelSensor_RR_Direct;


  extern int rawLevelSensor_FL_Direct;
  extern int rawLevelSensor_FR_Direct;
  extern int rawLevelSensor_RL_Direct;
  extern int rawLevelSensor_RR_Direct;

  extern int newLevelSensor_FL_Direct;
  extern int newLevelSensor_FR_Direct;
  extern int newLevelSensor_RL_Direct;
  extern int newLevelSensor_RR_Direct;



  extern int FL_Height_Direct; // Front Left Ride Height (mm)
  extern int FR_Height_Direct; // Front Right Ride Height (mm)
  extern int RL_Height_Direct; // Rear Left Ride Height (mm)
  extern int RR_Height_Direct; // Rear Right Ride Height (mm)

  extern unsigned int UnsignedLevelSensor_FL_Direct;
  extern unsigned int UnsignedLevelSensor_FR_Direct;
  extern unsigned int UnsignedLevelSensor_RL_Direct;
  extern unsigned int UnsignedLevelSensor_RR_Direct;
 
  extern unsigned int UnsignedLevelSensor_withOffset_FL_Direct;
  extern unsigned int UnsignedLevelSensor_withOffset_FR_Direct;
  extern unsigned int UnsignedLevelSensor_withOffset_RL_Direct;
  extern unsigned int UnsignedLevelSensor_withOffset_RR_Direct; 

  extern float FrequencyFL, FrequencyFR, FrequencyRL, FrequencyRR;
  extern float periodFL, periodFR, periodRL, periodRR;
  extern float DutyCycleFL, DutyCycleFR, DutyCycleRL, DutyCycleRR;

  extern float sumFL, sumFR, sumRL, sumRR;
  extern int countFL, countFR, countRL, countRR;


  extern unsigned int InstructionFromMaster_Susp_LvlSnsr_Offsets_Reset;

  extern  unsigned int FreqFL_Int, FreqFR_Int, FreqRL_Int, FreqRR_Int;
  extern signed int FreqFL_IntDelta, FreqFR_IntDelta, FreqRL_IntDelta, FreqRR_IntDelta;
  extern unsigned int FreqFL_IntNew, FreqFR_IntNew, FreqRL_IntNew, FreqRR_IntNew;
  extern unsigned int FreqFL_IntLast, FreqFR_IntLast, FreqRL_IntLast, FreqRR_IntLast;
  extern unsigned int FreqFL_DirectionOfTravel_UP, FreqFR_DirectionOfTravel_UP, FreqRL_DirectionOfTravel_UP, FreqRR_DirectionOfTravel_UP;
  extern unsigned int FreqFL_DirectionOfTravel_DOWN, FreqFR_DirectionOfTravel_DOWN, FreqRL_DirectionOfTravel_DOWN, FreqRR_DirectionOfTravel_DOWN;



  extern signed int FreqFL_Travel1, FreqFR_Travel1, FreqRL_Travel1, FreqRR_Travel1;
  extern signed int FreqFL_StaticHeight1, FreqFR_StaticHeight1, FreqRL_StaticHeight1, FreqRR_StaticHeight1;
  extern signed int FreqFL_CurrentHeight, FreqFR_CurrentHeight, FreqRL_CurrentHeight, FreqRR_CurrentHeight;
  extern signed int FreqFL_CurrentHeightOffset, FreqFR_CurrentHeightOffset, FreqRL_CurrentHeightOffset, FreqRR_CurrentHeightOffset;

  extern elapsedMillis ResetTimeout;

#pragma endregion

#pragma region // EEPROM Variables - Raw

extern unsigned int eeprom0; // IMU Offset AccelX
extern unsigned int eeprom1; // IMU Offset AccelX
extern unsigned int eeprom2; // IMU Offset AccelY
extern unsigned int eeprom3; // IMU Offset AccelY
extern unsigned int eeprom4; // IMU Offset AccelZ
extern unsigned int eeprom5; // IMU Offset AccelZ
 
extern unsigned int eeprom6; // IMU Gyro AccelX
extern unsigned int eeprom7; // IMU Gyro AccelX
extern unsigned int eeprom8; // IMU Gyro AccelY
extern unsigned int eeprom9; // IMU Gyro AccelY
extern unsigned int eeprom10; // IMU Gyro AccelZ
extern unsigned int eeprom11; // IMU Gyro AccelZ

extern unsigned int eeprom20; // Suspension Level Sensor_FL_LSB
extern unsigned int eeprom21; // Suspension Level Sensor_FL_LSB
extern unsigned int eeprom22; // Suspension Level Sensor_FR_LSB
extern unsigned int eeprom23; // Suspension Level Sensor_FR_LSB
extern unsigned int eeprom24; // Suspension Level Sensor_RL_LSB
extern unsigned int eeprom25; // Suspension Level Sensor_RL_LSB
extern unsigned int eeprom26; // Suspension Level Sensor_RR_LSB
extern unsigned int eeprom27; // Suspension Level Sensor_RR_LSB

extern unsigned int eeprom28; // CarConfig Suspension LevelSensors;

// ------------------------------------------------------------------------------------------------------------------
//KalmanFilters
extern unsigned int EEPROM_val_LSB_MQB_AutoShift_EngineRPM_KalmanFilteredMPC_Gain_Int; // Stored MQB Autoshift EngineRPM KalmanFilteredMPC_Gain (this is divided by 100 to arrive at this Float)

// -----------------------------------------------------------------------------------------------------------------


extern unsigned int EEPROM_val_LSB_storedAutoUPShift_EngineRPM_Setpoint12; // Stored AutoUPshift StoredRPM Gear 1-2 LSB
extern unsigned int EEPROM_val_MSB_storedAutoUPShift_EngineRPM_Setpoint12; // Stored AutoUPshift StoredRPM Gear 1-2 MSB
extern unsigned int EEPROM_val_LSB_storedAutoUPShift_EngineRPM_Setpoint23; // Stored AutoUPshift StoredRPM Gear 2-3 LSB
extern unsigned int EEPROM_val_MSB_storedAutoUPShift_EngineRPM_Setpoint23; // Stored AutoUPshift StoredRPM Gear 2-3 MSB
extern unsigned int EEPROM_val_LSB_storedAutoUPShift_EngineRPM_Setpoint34; // Stored AutoUPshift StoredRPM Gear 3-4 LSB
extern unsigned int EEPROM_val_MSB_storedAutoUPShift_EngineRPM_Setpoint34; // Stored AutoUPshift StoredRPM Gear 3-4 MSB
extern unsigned int EEPROM_val_LSB_storedAutoUPShift_EngineRPM_Setpoint45; // Stored AutoUPshift StoredRPM Gear 4-5 LSB
extern unsigned int EEPROM_val_MSB_storedAutoUPShift_EngineRPM_Setpoint45; // Stored AutoUPshift StoredRPM Gear 4-5 MSB
extern unsigned int EEPROM_val_LSB_storedAutoUPShift_EngineRPM_Setpoint56; // Stored AutoUPshift StoredRPM Gear 5-6 LSB
extern unsigned int EEPROM_val_MSB_storedAutoUPShift_EngineRPM_Setpoint56; // Stored AutoUPshift StoredRPM Gear 5-6 MSB
extern unsigned int EEPROM_val_LSB_storedAutoUPShift_EngineRPM_Setpoint67; // Stored AutoUPshift StoredRPM Gear 6-7 LSB
extern unsigned int EEPROM_val_MSB_storedAutoUPShift_EngineRPM_Setpoint67; // Stored AutoUPshift StoredRPM Gear 6-7 MSB

extern unsigned int EEPROM_val_LSB_storedAutoDOWNshift_EngineRPM_Setpoint21; // Stored AutoDOWNshift StoredRPM Gear 2-1 LSB
extern unsigned int EEPROM_val_MSB_storedAutoDOWNshift_EngineRPM_Setpoint21; // Stored AutoDOWNshift StoredRPM Gear 2-1 MSB
extern unsigned int EEPROM_val_LSB_storedAutoDOWNshift_EngineRPM_Setpoint32; // Stored AutoDOWNshift StoredRPM Gear 3-2 LSB
extern unsigned int EEPROM_val_MSB_storedAutoDOWNshift_EngineRPM_Setpoint32; // Stored AutoDOWNshift StoredRPM Gear 3-2 MSB
extern unsigned int EEPROM_val_LSB_storedAutoDOWNshift_EngineRPM_Setpoint43; // Stored AutoDOWNshift StoredRPM Gear 4-3 LSB
extern unsigned int EEPROM_val_MSB_storedAutoDOWNshift_EngineRPM_Setpoint43; // Stored AutoDOWNshift StoredRPM Gear 4-3 MSB
extern unsigned int EEPROM_val_LSB_storedAutoDOWNshift_EngineRPM_Setpoint54; // Stored AutoDOWNshift StoredRPM Gear 5-4 LSB
extern unsigned int EEPROM_val_MSB_storedAutoDOWNshift_EngineRPM_Setpoint54; // Stored AutoDOWNshift StoredRPM Gear 5-4 MSB
extern unsigned int EEPROM_val_LSB_storedAutoDOWNshift_EngineRPM_Setpoint65; // Stored AutoDOWNshift StoredRPM Gear 6-5 LSB
extern unsigned int EEPROM_val_MSB_storedAutoDOWNshift_EngineRPM_Setpoint65; // Stored AutoDOWNshift StoredRPM Gear 6-5 MSB
extern unsigned int EEPROM_val_LSB_storedAutoDOWNshift_EngineRPM_Setpoint76; // Stored AutoDOWNshift StoredRPM Gear 7-6 LSB
extern unsigned int EEPROM_val_MSB_storedAutoDOWNshift_EngineRPM_Setpoint76; // Stored AutoDOWNshift StoredRPM Gear 7-6 MSB 


extern unsigned int EEPROM_val_LSB_storedLC_EngineRPM_Setpoint_Ramp; // Stored LC Target EngineRPM Ramp Setpoint LSB
extern unsigned int EEPROM_val_MSB_storedLC_EngineRPM_Setpoint_Ramp; // Stored LC Target EngineRPM Ramp Setpoint MSB
extern unsigned int EEPROM_val_LSB_storedLC_EngineRPM_Setpoint_Hold; // Stored LC Target EngineRPM Hold Setpoint LSB
extern unsigned int EEPROM_val_MSB_storedLC_EngineRPM_Setpoint_Hold; // Stored LC Target EngineRPM Hold Setpoint MSB

extern unsigned int EEPROM_val_LSB_storedLC_EngineRPM_RAMP_Timeout; // Stored LC Target EngineRPM Ramp Setpoint LSB


extern unsigned int EEPROM_val_LSB_storedLC_EngineRPM_Timeout; // Stored LC Target EngineRPM Ramp Setpoint LSB
extern unsigned int EEPROM_val_MSB_storedLC_EngineRPM_Timeout; // Stored LC Target EngineRPM Ramp Setpoint MSB

extern unsigned int EEPROM_val_LSB_storedLC_EngineRPM_TransitionRateMultiplier_Int;  // Stored LC EngineRPM_TransitionRateMultiplier_LSB
extern unsigned int EEPROM_val_MSB_storedLC_EngineRPM_TransitionRateMultiplier_Int;  // Stored LC EngineRPM_TransitionRateMultiplier_MSB

extern unsigned int EEPROM_val_LSB_storedLC_EngineRPM_PID_BaseOffset_Int;  // Stored LC EngineRPM_PID_BaseOffset_Int_LSB // this is in the range of 0-20 (a value of 30 is subtracted from this to arrive at the actual offset used )
// extern signed int EEPROM_val_MSB_storedMQB_LaunchControl_EngineRPM_PID_BaseOffset_Int;  // Stored LC EngineRPM_PID_BaseOffset_Int_MSB

extern unsigned int EEPROM_val_LSB_storedLC_EngineRPM_PID_Ki_Term_Int;  // Stored LC EngineRPM_PID_iTerm_Int (This is the Ki term multiplied by 100)


extern unsigned int EEPROM_val_LSB_storedLC_BoostWindow_EntryThresholdPressure; // Stored LC BoostWindow_EntryThresholdPressure_LSB
extern unsigned int EEPROM_val_MSB_storedLC_BoostWindow_EntryThresholdPressure; // Stored LC BoostWindow_EntryThresholdPressure_MSB
extern unsigned int EEPROM_val_LSB_storedLC_BoostWindow_minMaintainPressure; // Stored LC BoostWindow_minMaintainPressure_LSB
extern unsigned int EEPROM_val_MSB_storedLC_BoostWindow_minMaintainPressure; // Stored LC BoostWindow_minMaintainPressure_MSB
extern unsigned int EEPROM_val_LSB_storedLC_EngineRPM_BoostVariableSelection; // Stored LC BoostVariableSelection Setpoint LSB
 
extern unsigned int EEPROM_val_LSB_storedTC_TgtSlipRate; // Stored TractionControl_TgtSlipRate_LSB
extern unsigned int EEPROM_val_MSB_storedTC_TgtSlipRate; // Stored TractionControl_TgtSlipRate_MSB
extern unsigned int EEPROM_val_LSB_storedTC_CorrectionAggression; // Stored TractionControl_CorrectionAggression_LSB
extern unsigned int EEPROM_val_MSB_storedTC_CorrectionAggression; // Stored TractionControl_CorrectionAggression_MSB

extern unsigned int EEPROM_val_LSB_storedBurnoutMode_EngineRPM_Setpoint; // Stored BurnoutMode EngineRPM Setpoint LSB
extern unsigned int EEPROM_val_MSB_storedBurnoutMode_EngineRPM_Setpoint; // Stored BurnoutMode EngineRPM Setpoint MSB

extern unsigned int EEPROM_val_LSB_storedTemp7_Type;
extern unsigned int EEPROM_val_LSB_storedTemp8_Type;


extern unsigned int EEPROM_val_LSB_storedMQB_CarConfig_SuspLevelSensors;
extern unsigned int EEPROM_val_LSB_storedMQB_CarConfig_AWDController;
extern unsigned int EEPROM_val_LSB_storedMQB_CarConfig_DisplayGaugesOverride;



extern unsigned int EEPROM_val_LSB_storedMQB_Parachute_PreArmedStatus; // Stored Parachute_PreArmedStatus LSB
extern unsigned int EEPROM_val_LSB_storedMQB_Parachute_ArmSpeed_Setpoint; // Stored Parachute_ArmSpeed Setpoint LSB
extern unsigned int EEPROM_val_MSB_storedMQB_Parachute_ArmSpeed_Setpoint; // Stored Parachute_ArmSpeed Setpoint MSB



extern unsigned int EEPROM_val_LSB_MQB_BoostByGearMultiplier_ActivationStatus; // Stored MQB_BoostByGearMultiplier_ActivationStatus LSB
extern unsigned int EEPROM_val_LSB_MQB_BoostByGearMultiplier_Gear1; // Stored MQB_BoostByGearMultiplier_Gear1 LSB
extern unsigned int EEPROM_val_LSB_MQB_BoostByGearMultiplier_Gear2; // Stored MQB_BoostByGearMultiplier_Gear2 LSB
extern unsigned int EEPROM_val_LSB_MQB_BoostByGearMultiplier_Gear3; // Stored MQB_BoostByGearMultiplier_Gear3 LSB
extern unsigned int EEPROM_val_LSB_MQB_BoostByGearMultiplier_Gear4; // Stored MQB_BoostByGearMultiplier_Gear4 LSB
extern unsigned int EEPROM_val_LSB_MQB_BoostByGearMultiplier_Gear5; // Stored MQB_BoostByGearMultiplier_Gear5 LSB
extern unsigned int EEPROM_val_LSB_MQB_BoostByGearMultiplier_Gear6; // Stored MQB_BoostByGearMultiplier_Gear6 LSB
extern unsigned int EEPROM_val_LSB_MQB_BoostByGearMultiplier_Gear7; // Stored MQB_BoostByGearMultiplier_Gear7 LSB


// LS Outputs -----------------------------------------------------------------------------------------------------------------------------

  extern unsigned int EEPROM_val_LSB_LS1_Output_Type; // Stored LS1 Output Type
  extern unsigned int EEPROM_val_LSB_LS2_Output_Type; // Stored LS2 Output Type
  extern unsigned int EEPROM_val_LSB_LS3_Output_Type; // Stored LS3 Output Type
  extern unsigned int EEPROM_val_LSB_LS4_Output_Type; // Stored LS4 Output Type
  extern unsigned int EEPROM_val_LSB_LS5_Output_Type; // Stored LS5 Output Type
  extern unsigned int EEPROM_val_LSB_LS6_Output_Type; // Stored LS6 Output Type
  extern unsigned int EEPROM_val_LSB_HS1_Output_Type; // Stored LS6 Output Type

  extern unsigned int EEPROM_val_LSB_storedLS1_Output_ActivationTimer_Setpoint_ms; // Stored ActivationTimer Setpoint for LS1
  extern unsigned int EEPROM_val_LSB_storedLS2_Output_ActivationTimer_Setpoint_ms; // Stored ActivationTimer Setpoint for LS2
  extern unsigned int EEPROM_val_LSB_storedLS3_Output_ActivationTimer_Setpoint_ms; // Stored ActivationTimer Setpoint for LS3
  extern unsigned int EEPROM_val_LSB_storedLS4_Output_ActivationTimer_Setpoint_ms; // Stored ActivationTimer Setpoint for LS4
  extern unsigned int EEPROM_val_LSB_storedLS5_Output_ActivationTimer_Setpoint_ms; // Stored ActivationTimer Setpoint for LS5
  extern unsigned int EEPROM_val_LSB_storedLS6_Output_ActivationTimer_Setpoint_ms; // Stored ActivationTimer Setpoint for LS6
  extern unsigned int EEPROM_val_LSB_storedHS1_Output_ActivationTimer_Setpoint_ms; // Stored ActivationTimer Setpoint for LS6


  extern unsigned int EEPROM_val_LSB_storedLS1_Output_PWMFreq; // Stored PWM Frequency for LS1 and LS2
  extern unsigned int EEPROM_val_LSB_storedLS2_Output_PWMFreq; // Stored PWM Frequency for LS1 and LS2
  extern unsigned int EEPROM_val_LSB_storedLS3_Output_PWMFreq; // Stored PWM Frequency for LS3
  extern unsigned int EEPROM_val_LSB_storedLS4_Output_PWMFreq; // Stored PWM Frequency for LS4
  extern unsigned int EEPROM_val_LSB_storedLS5_Output_PWMFreq; // Stored PWM Frequency for LS5
  extern unsigned int EEPROM_val_LSB_storedLS6_Output_PWMFreq; // Stored PWM Frequency for LS6
  extern unsigned int EEPROM_val_LSB_storedHS1_Output_PWMFreq; // Stored PWM Frequency for LS6




#pragma endregion

#pragma region // Stored EEPROM Data 

extern signed int storedOffsetAccelX;
extern signed int storedOffsetAccelY;
extern signed int storedOffsetAccelZ;

extern signed int initialOffsetAccelX;
extern signed int initialOffsetAccelY;
extern signed int initialOffsetAccelZ;

extern signed int storedOffsetGyroX;
extern signed int storedOffsetGyroY;
extern signed int storedOffsetGyroZ;

extern signed int initialOffsetGyroX;
extern signed int initialOffsetGyroY;
extern signed int initialOffsetGyroZ;

// -----------------------------------------------------------------------------------------------------------------------------

extern signed int storedOffset_LevelSensor_FL;
extern signed int storedOffset_LevelSensor_FR;
extern signed int storedOffset_LevelSensor_RL;
extern signed int storedOffset_LevelSensor_RR;

extern signed int initialOffset_LevelSensor_FL;
extern signed int initialOffset_LevelSensor_FR;
extern signed int initialOffset_LevelSensor_RL;
extern signed int initialOffset_LevelSensor_RR;

// -----------------------------------------------------------------------------------------------------------------------------

extern unsigned int initialMQB_AutoUPshift_Gear1to2_RPMSetpoint_Int;
extern unsigned int initialMQB_AutoUPshift_Gear2to3_RPMSetpoint_Int;
extern unsigned int initialMQB_AutoUPshift_Gear3to4_RPMSetpoint_Int;
extern unsigned int initialMQB_AutoUPshift_Gear4to5_RPMSetpoint_Int;
extern unsigned int initialMQB_AutoUPshift_Gear5to6_RPMSetpoint_Int;
extern unsigned int initialMQB_AutoUPshift_Gear6to7_RPMSetpoint_Int;

extern unsigned int storedMQB_AutoUPshift_Gear1to2_RPMSetpoint_Int;
extern unsigned int storedMQB_AutoUPshift_Gear2to3_RPMSetpoint_Int;
extern unsigned int storedMQB_AutoUPshift_Gear3to4_RPMSetpoint_Int;
extern unsigned int storedMQB_AutoUPshift_Gear4to5_RPMSetpoint_Int; 
extern unsigned int storedMQB_AutoUPshift_Gear5to6_RPMSetpoint_Int;
extern unsigned int storedMQB_AutoUPshift_Gear6to7_RPMSetpoint_Int;

// -----------------------------------------------------------------------------------------------------------------------------


extern unsigned int initialMQB_AutoDOWNshift_Gear2to1_RPMSetpoint_Int;
extern unsigned int initialMQB_AutoDOWNshift_Gear3to2_RPMSetpoint_Int;
extern unsigned int initialMQB_AutoDOWNshift_Gear4to3_RPMSetpoint_Int;
extern unsigned int initialMQB_AutoDOWNshift_Gear5to4_RPMSetpoint_Int;
extern unsigned int initialMQB_AutoDOWNshift_Gear6to5_RPMSetpoint_Int;
extern unsigned int initialMQB_AutoDOWNshift_Gear7to6_RPMSetpoint_Int;

extern unsigned int storedMQB_AutoDOWNshift_Gear2to1_RPMSetpoint_Int;
extern unsigned int storedMQB_AutoDOWNshift_Gear3to2_RPMSetpoint_Int;
extern unsigned int storedMQB_AutoDOWNshift_Gear4to3_RPMSetpoint_Int;
extern unsigned int storedMQB_AutoDOWNshift_Gear5to4_RPMSetpoint_Int;
extern unsigned int storedMQB_AutoDOWNshift_Gear6to5_RPMSetpoint_Int;
extern unsigned int storedMQB_AutoDOWNshift_Gear7to6_RPMSetpoint_Int;

// -----------------------------------------------------------------------------------------------------------------------------

extern unsigned int initialMQB_LaunchControl_EngineRPM_Ramp_Setpoint_Int;
extern unsigned int initialMQB_LaunchControl_EngineRPM_Hold_Setpoint_Int;
extern unsigned int initialMQB_LaunchControl_BoostWindow_EntryThresholdPressure_Int;
extern unsigned int initialMQB_LaunchControl_BoostWindow_minMaintainPressure_Int;

extern unsigned int storedMQB_LaunchControl_EngineRPM_Ramp_Setpoint_Int;
extern unsigned int storedMQB_LaunchControl_EngineRPM_Hold_Setpoint_Int;
extern unsigned int storedMQB_LaunchControl_BoostWindow_EntryThresholdPressure_Int;
extern unsigned int storedMQB_LaunchControl_BoostWindow_minMaintainPressure_Int;

extern unsigned int storedMQB_LaunchControl_EngineRPM_BoostVariableSelection_Int; // Stored LC MQB BoostVariableSelction (Boost Pressure or TurboRPM)
extern unsigned int initialMQB_LaunchControl_EngineRPM_BoostVariableSelection_Int; // Stored LC MQB BoostVariableSelction (Boost Pressure or TurboRPM)

extern unsigned int storedMQB_LaunchControl_EngineRPM_TransitionRateMultiplier_Int;
extern unsigned int initialMQB_LaunchControl_EngineRPM_TransitionRateMultiplier_Int;

extern unsigned int storedMQB_LaunchControl_EngineRPM_RAMP_Timeout_Int; // Stored LC MQB EngineRPM Transtion Rate Multiplier_LSB
extern unsigned int initialMQB_LaunchControl_EngineRPM_RAMP_Timeout_Int; // Stored LC MQB EngineRPM Transtion Rate Multiplier_LSB


// -----------------------------------------------------------------------------------------------------------------------------

extern unsigned int storedMQB_LaunchControl_EngineRPM_PID_BaseOffset_Int; // Stored MQB LC EngineRPM Base Offset (Signed Int) // this is in the range of 0-20 (a value of 30 is subtracted from this to arrive at the actual offset used )
// extern unsigned int initialMQB_LaunchControl_EngineRPM_PID_BaseOffset_Int; /// Stored MQB LC EngineRPM Base Offset (Signed Int)

extern unsigned int storedMQB_LaunchControl_EngineRPM_PID_Ki_Term_Int; // Stored MQB LC EngineRPM PID iTerm (This is multiplied by 100 to for actual KI Term)
extern unsigned int initialMQB_LaunchControl_EngineRPM_PID_Ki_Term_Int; // Stored MQB LC EngineRPM PID iTerm (This is multiplied by 100 to for actual KI Term)


// -----------------------------------------------------------------------------------------------------------------------------

 extern unsigned int storedMQB_TractionControl_TgtSlipRate_Int;
 extern unsigned int storedMQB_TractionControl_CorrectionAggression_Int;

 extern unsigned int initialMQB_TractionControl_TgtSlipRate_Int;
 extern unsigned int initialMQB_TractionControl_CorrectionAggression_Int;

// -----------------------------------------------------------------------------------------------------------------------------

extern unsigned int initialMQB_BurnoutMode_EngineRPM_Setpoint_Int;
extern unsigned int storedMQB_BurnoutMode_EngineRPM_Setpoint_Int;

// -----------------------------------------------------------------------------------------------------------------------------

extern unsigned int storedTemp7_Type;
extern unsigned int initialTemp7_Type;
extern unsigned int storedTemp8_Type;
extern unsigned int initialTemp8_Type;
// -----------------------------------------------------------------------------------------------------------------------------

extern unsigned int storedMQB_CarConfig_SuspLevelSensors;
extern unsigned int initialMQB_CarConfig_SuspLevelSensors;

extern unsigned int storedMQB_CarConfig_AWDController;
extern unsigned int initialMQB_CarConfig_AWDController;

extern unsigned int storedMQB_CarConfig_DisplayGaugesOverride;
extern unsigned int initialMQB_CarConfig_DisplayGaugesOverride;


// -----------------------------------------------------------------------------------------------------------------------------

extern unsigned int storedMQB_Parachute_ArmSpeed_Setpoint_Int;
extern unsigned int initialMQB_Parachute_ArmSpeed_Setpoint_Int;

// -----------------------------------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------------------------------

extern unsigned int storedLS1_Output_Type;
extern unsigned int storedLS2_Output_Type;
extern unsigned int storedLS3_Output_Type;
extern unsigned int storedLS4_Output_Type;
extern unsigned int storedLS4_Output_Type;
extern unsigned int storedLS5_Output_Type;
extern unsigned int storedLS6_Output_Type;
extern unsigned int storedHS1_Output_Type;

extern unsigned int initialLS1_Output_Type;
extern unsigned int initialLS2_Output_Type;
extern unsigned int initialLS3_Output_Type;
extern unsigned int initialLS4_Output_Type;
extern unsigned int initialLS5_Output_Type;
extern unsigned int initialLS6_Output_Type;
extern unsigned int initialHS1_Output_Type;

extern unsigned int storedLS1_Output_ActivationTimer_Setpoint_ms;
extern unsigned int storedLS2_Output_ActivationTimer_Setpoint_ms;
extern unsigned int storedLS3_Output_ActivationTimer_Setpoint_ms;
extern unsigned int storedLS4_Output_ActivationTimer_Setpoint_ms;
extern unsigned int storedLS5_Output_ActivationTimer_Setpoint_ms;
extern unsigned int storedLS6_Output_ActivationTimer_Setpoint_ms;
extern unsigned int storedHS1_Output_ActivationTimer_Setpoint_ms;

extern unsigned int initialLS1_Output_ActivationTimer_Setpoint_ms;
extern unsigned int initialLS2_Output_ActivationTimer_Setpoint_ms;
extern unsigned int initialLS3_Output_ActivationTimer_Setpoint_ms;
extern unsigned int initialLS4_Output_ActivationTimer_Setpoint_ms;
extern unsigned int initialLS5_Output_ActivationTimer_Setpoint_ms;
extern unsigned int initialLS6_Output_ActivationTimer_Setpoint_ms;
extern unsigned int initialHS1_Output_ActivationTimer_Setpoint_ms;


extern unsigned int storedLS1_Output_PWMFreq;
extern unsigned int storedLS2_Output_PWMFreq;
extern unsigned int storedLS3_Output_PWMFreq;
extern unsigned int storedLS4_Output_PWMFreq;
extern unsigned int storedLS5_Output_PWMFreq;
extern unsigned int storedLS6_Output_PWMFreq;
extern unsigned int storedHS1_Output_PWMFreq;

extern unsigned int initialLS1_Output_PWMFreq;
extern unsigned int initialLS2_Output_PWMFreq;
extern unsigned int initialLS3_Output_PWMFreq;
extern unsigned int initialLS4_Output_PWMFreq;
extern unsigned int initialLS5_Output_PWMFreq;
extern unsigned int initialLS6_Output_PWMFreq;
extern unsigned int initialHS1_Output_PWMFreq;

// -----------------------------------------------------------------------------------------------------------------------------

extern unsigned int storedMQB_ConfigurationMode_JobNumber_to_Update;
extern unsigned int initialMQB_ConfigurationMode_JobNumber_to_Update;

// -----------------------------------------------------------------------------------------------------------------------------



extern unsigned int storedMQB_BoostByGearMultiplier_ActivationStatus;
extern unsigned int storedMQB_BoostByGearMultiplier_Gear1;
extern unsigned int storedMQB_BoostByGearMultiplier_Gear2;
extern unsigned int storedMQB_BoostByGearMultiplier_Gear3;
extern unsigned int storedMQB_BoostByGearMultiplier_Gear4;
extern unsigned int storedMQB_BoostByGearMultiplier_Gear5;
extern unsigned int storedMQB_BoostByGearMultiplier_Gear6;
extern unsigned int storedMQB_BoostByGearMultiplier_Gear7;

extern unsigned int iniitalMQB_BoostByGearMultiplier_ActivationStatus;
extern unsigned int iniitalMQB_BoostByGearMultiplier_Gear1;
extern unsigned int iniitalMQB_BoostByGearMultiplier_Gear2;
extern unsigned int iniitalMQB_BoostByGearMultiplier_Gear3;
extern unsigned int iniitalMQB_BoostByGearMultiplier_Gear4;
extern unsigned int iniitalMQB_BoostByGearMultiplier_Gear5;
extern unsigned int iniitalMQB_BoostByGearMultiplier_Gear6;
extern unsigned int iniitalMQB_BoostByGearMultiplier_Gear7;


// -----------------------------------------------------------------------------------------------------------------------------


extern unsigned int storedLS1_Output_ActivationTimer_Setpoint_ms; // Stored ActivationTimer Setpoint for LS1
extern unsigned int storedLS2_Output_ActivationTimer_Setpoint_ms; // Stored ActivationTimer Setpoint for LS2
extern unsigned int storedLS3_Output_ActivationTimer_Setpoint_ms; // Stored ActivationTimer Setpoint for LS3
extern unsigned int storedLS4_Output_ActivationTimer_Setpoint_ms; // Stored ActivationTimer Setpoint for LS4
extern unsigned int storedLS5_Output_ActivationTimer_Setpoint_ms; // Stored ActivationTimer Setpoint for LS5
extern unsigned int storedLS6_Output_ActivationTimer_Setpoint_ms; // Stored ActivationTimer Setpoint for LS6
extern unsigned int storedHS1_Output_ActivationTimer_Setpoint_ms; // Stored ActivationTimer Setpoint for LS6

extern unsigned int initialLS1_Output_ActivationTimer_Setpoint_ms; // Stored ActivationTimer Setpoint for LS1
extern unsigned int initialLS2_Output_ActivationTimer_Setpoint_ms; // Stored ActivationTimer Setpoint for LS2
extern unsigned int initialLS3_Output_ActivationTimer_Setpoint_ms; // Stored ActivationTimer Setpoint for LS3
extern unsigned int initialLS4_Output_ActivationTimer_Setpoint_ms; // Stored ActivationTimer Setpoint for LS4
extern unsigned int initialLS5_Output_ActivationTimer_Setpoint_ms; // Stored ActivationTimer Setpoint for LS5
extern unsigned int initialLS6_Output_ActivationTimer_Setpoint_ms; // Stored ActivationTimer Setpoint for LS6
extern unsigned int initialHS1_Output_ActivationTimer_Setpoint_ms; // Stored ActivationTimer Setpoint for LS6



extern unsigned int storedLSB_storedLS1_Output_PWMFreq; // Stored PWM Frequency for LS1 and LS2
extern unsigned int storedLSB_storedLS2_Output_PWMFreq; // Stored PWM Frequency for LS1 and LS2
extern unsigned int storedLSB_storedLS3_Output_PWMFreq; // Stored PWM Frequency for LS3
extern unsigned int storedLSB_storedLS4_Output_PWMFreq; // Stored PWM Frequency for LS4
extern unsigned int storedLSB_storedLS5_Output_PWMFreq; // Stored PWM Frequency for LS5
extern unsigned int storedLSB_storedLS6_Output_PWMFreq; // Stored PWM Frequency for LS6
extern unsigned int storedLSB_storedHS1_Output_PWMFreq; // Stored PWM Frequency for LS6

extern unsigned int initialLSB_storedLS1_Output_PWMFreq; // Stored PWM Frequency for LS1 and LS2
extern unsigned int initialLSB_storedLS2_Output_PWMFreq; // Stored PWM Frequency for LS1 and LS2
extern unsigned int initialLSB_storedLS3_Output_PWMFreq; // Stored PWM Frequency for LS3
extern unsigned int initialLSB_storedLS4_Output_PWMFreq; // Stored PWM Frequency for LS4
extern unsigned int initialLSB_storedLS5_Output_PWMFreq; // Stored PWM Frequency for LS5
extern unsigned int initialLSB_storedLS6_Output_PWMFreq; // Stored PWM Frequency for LS6
extern unsigned int initialLSB_storedHS1_Output_PWMFreq; // Stored PWM Frequency for LS6





#pragma endregion

#pragma region // FlexCAN Variables - PT-CAN  (primarily variables read from PT-CAN sniffing - Operational and Driver Inputs)

extern FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> TFTCAN1;
extern FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> TFTCAN2;
extern FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16> TFTCAN3;

// Continous TFT Generated CAN Frames on PT-CAN ------------------------------------------------------------------------
extern CAN_message_t PT_CAN_msg0x780;
extern CAN_message_t PT_CAN_msg0x781_mux1; 
extern CAN_message_t PT_CAN_msg0x781_mux2; 
extern CAN_message_t PT_CAN_msg0x781_mux3; 
extern CAN_message_t PT_CAN_msg0x781_mux4; 
extern CAN_message_t PT_CAN_msg0x781_mux5; 
extern CAN_message_t PT_CAN_msg0x782;
extern CAN_message_t PT_CAN_msg0x783;

extern CAN_message_t PT_CAN_msg0x784;
extern CAN_message_t PT_CAN_msg0x784_mux1;
extern CAN_message_t PT_CAN_msg0x784_mux2;

extern CAN_message_t PT_CAN_msg0x785;
extern CAN_message_t PT_CAN_msg0x785_mux1;
extern CAN_message_t PT_CAN_msg0x785_mux2;
extern CAN_message_t PT_CAN_msg0x785_mux3;

extern CAN_message_t PT_CAN_msg0x786;
extern CAN_message_t PT_CAN_msg0x786_mux1;
extern CAN_message_t PT_CAN_msg0x786_mux2;
extern CAN_message_t PT_CAN_msg0x786_mux3;

extern CAN_message_t PT_CAN_msg0x787_mux1;
extern CAN_message_t PT_CAN_msg0x787_mux2;
extern CAN_message_t PT_CAN_msg0x787_mux3;

extern CAN_message_t PT_CAN_msg0x794_mux1;
extern CAN_message_t PT_CAN_msg0x794_mux2;
extern CAN_message_t PT_CAN_msg0x794_mux3;
extern CAN_message_t PT_CAN_msg0x794_mux4;
extern CAN_message_t PT_CAN_msg0x794_mux5;

extern CAN_message_t PT_CAN_msg0x795;
extern CAN_message_t PT_CAN_msg0x796;



extern CAN_message_t OBD_CAN_msg0x794_mux1;
extern CAN_message_t OBD_CAN_msg0x794_mux2;
extern CAN_message_t OBD_CAN_msg0x794_mux3;
extern CAN_message_t OBD_CAN_msg0x794_mux4;
extern CAN_message_t OBD_CAN_msg0x794_mux5;

extern unsigned int OBD_CAN_msg0x794_muxLevel;



// Continous DS1 Generated CAN Frames on PT-CAN ------------------------------------------------------------------------
extern CAN_message_t PT_CAN_msg0x743; // DS1 CAN Frame 1 sent via ECU
extern CAN_message_t PT_CAN_msg0x747; // DS1 CAN Frame 2 sent via ECU


// CAN Frames resulting from Service22 Requests on PT-CAN ------------------------------------------------------------------------
extern CAN_message_t PT_CAN_msg0x700;
extern CAN_message_t PT_CAN_msg0x779; // AWD - For Haldex variables
extern CAN_message_t PT_CAN_msg0x77A; // MultiFunction Steering Wheel (driver inputs/buttons)
extern CAN_message_t PT_CAN_msg0x77D; // ABS - For Dyno Mode and various AWD variables

extern CAN_message_t PT_CAN_msg0x7DC; // Susp. Electronics - Level Sensors
extern CAN_message_t PT_CAN_msg09;

extern CAN_message_t PT_CAN_msg0x7E8; // ECU
extern CAN_message_t PT_CAN_msg0x7E9; // TCU

extern CAN_message_t PT_CAN_msg0x40; // Ign Status (consistent/reliable timer generated from Airbag module
extern CAN_message_t PT_CAN_msg0x0A8; // Engine RPM
extern CAN_message_t PT_CAN_msg0xAD; // Gear Lever/Gear Position Status
extern CAN_message_t PT_CAN_msg0x0B2; // WheelSpeeds
extern CAN_message_t PT_CAN_msg0x104; // Parking Brake Status
extern CAN_message_t PT_CAN_msg0x107; // Tachometer EngineRPM Override address
extern CAN_message_t PT_CAN_msg0x670; // Tachometer Shift light in byte7?
extern CAN_message_t PT_CAN_msg0x147; // DisplayGauges
extern CAN_message_t PT_CAN_msg0x120; // Cruise Stalk Static Position
extern CAN_message_t PT_CAN_msg0x12B; // Cruise Stalk Movement Position/Status
extern CAN_message_t PT_CAN_msg0x1AB; // Brake Pedal Switch Status
extern CAN_message_t PT_CAN_msg0x366; // Hazard Lights
extern CAN_message_t PT_CAN_msg0x385; // Audi 8V Drive Select
extern CAN_message_t PT_CAN_msg0x31B; // ESP_24 - general vehicle speed and direction
extern CAN_message_t PT_CAN_msg0x391; // OBD/SAE Engine Basics (pedal, load, coolTemp, etc)
extern CAN_message_t PT_CAN_msg0x394; // DQ500 CurrentGear
extern CAN_message_t PT_CAN_msg0x3DD; // Paddle position/Shift status
extern CAN_message_t PT_CAN_msg0x7EE; // function to sniff DS1 Current MapSlot when RealDash is active (custom CAN Frames created from DS1)


extern unsigned int PT_CAN_3dd_buf0, PT_CAN_3dd_buf1, PT_CAN_3dd_buf2, PT_CAN_3dd_buf3, PT_CAN_3dd_buf4, PT_CAN_3dd_buf5, PT_CAN_3dd_buf6, PT_CAN_3dd_buf7; // 0x3DD is for paddle positions/shift status
extern unsigned int PT_CAN_147_buf0, PT_CAN_147_buf1, PT_CAN_147_buf2, PT_CAN_147_buf3, PT_CAN_147_buf4, PT_CAN_147_buf5, PT_CAN_147_buf6, PT_CAN_147_buf7;

extern String PT_CAN_VIN_buf1, PT_CAN_VIN_buf2, PT_CAN_VIN_buf3, PT_CAN_VIN_buf4, PT_CAN_VIN_buf5, PT_CAN_VIN_buf6, PT_CAN_VIN_buf7, PT_CAN_VIN_buf8;
extern String PT_CAN_VIN_buf9, PT_CAN_VIN_buf10, PT_CAN_VIN_buf11, PT_CAN_VIN_buf12, PT_CAN_VIN_buf13, PT_CAN_VIN_buf14, PT_CAN_VIN_buf15, PT_CAN_VIN_buf16, PT_CAN_VIN_buf17;
 
extern String PT_CAN_CALID_buf1, PT_CAN_CALID_buf2, PT_CAN_CALID_buf3, PT_CAN_CALID_buf4, PT_CAN_CALID_buf5, PT_CAN_CALID_buf6, PT_CAN_CALID_buf7, PT_CAN_CALID_buf8;
extern String PT_CAN_CALID_buf9, PT_CAN_CALID_buf10, PT_CAN_CALID_buf11, PT_CAN_CALID_buf12, PT_CAN_CALID_buf13, PT_CAN_CALID_buf14, PT_CAN_CALID_buf15, PT_CAN_CALID_buf16, PT_CAN_CALID_buf17;
 
extern String PT_CAN_CAL_buf1, PT_CAN_CAL_buf2, PT_CAN_CAL_buf3, PT_CAN_CAL_buf4, PT_CAN_CAL_buf5, PT_CAN_CAL_buf6, PT_CAN_CAL_buf7, PT_CAN_CAL_buf8, PT_CAN_CAL_buf9, PT_CAN_CAL_buf10;
extern String PT_CAN_CAL_buf11,CAL_buf12, PT_CAN_CAL_buf13, PT_CAN_CAL_buf14, PT_CAN_CAL_buf15, PT_CAN_CAL_buf16, PT_CAN_CAL_buf17;  // PT_CAN_CAL is maximum 16 ASCC characters.
 
extern String PT_CAN_ECUName_buf1, PT_CAN_ECUName_buf2, PT_CAN_ECUName_buf3, PT_CAN_ECUName_buf4, PT_CAN_ECUName_buf5, PT_CAN_ECUName_buf6, PT_CAN_ECUName_buf7, PT_CAN_ECUName_buf8;
extern String PT_CAN_ECUName_buf9, PT_CAN_ECUName_buf10, PT_CAN_ECUName_buf11, PT_CAN_ECUName_buf12, PT_CAN_ECUName_buf13, PT_CAN_ECUName_buf14, PT_CAN_ECUName_buf15, PT_CAN_ECUName_buf16, PT_CAN_ECUName_buf17;
extern String PT_CAN_ECUName_buf18, PT_CAN_ECUName_buf19, PT_CAN_ECUName_buf20, PT_CAN_ECUName_buf21, PT_CAN_ECUName_buf22, PT_CAN_ECUName_buf23, PT_CAN_ECUName_buf24, PT_CAN_ECUName_buf25, PT_CAN_ECUName_buf26, PT_CAN_ECUName_buf27;




#pragma endregion

#pragma region // FlexCAN Variables - OBD_CAN  (primarily variables used for Service22 Requests/Responses - Operational and Driver Inputs)

#ifndef Concept_TFT_StandaloneHaldexController
// extern FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> TFTCAN2;
#endif
#ifdef Concept_TFT_StandaloneHaldexController
extern FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> TFTCAN1;

#endif

extern CAN_message_t OBD_CAN_msg0x700;
extern CAN_message_t OBD_CAN_msg0x779; // AWD - For Haldex variables
extern CAN_message_t OBD_CAN_msg0x77A; // MultiFunction Steering Wheel (driver inputs/buttons)
extern CAN_message_t OBD_CAN_msg0x77D; // ABS - For Dyno Mode and various AWD variables

extern CAN_message_t OBD_CAN_msg0x7DC; // Susp. Electronics - Level Sensors
extern CAN_message_t OBD_CAN_msg09;

extern CAN_message_t OBD_CAN_msg0x7E8; // ECU
extern CAN_message_t OBD_CAN_msg0x7E9; // TCU


extern String OBD_CAN_VIN_buf1, OBD_CAN_VIN_buf2, OBD_CAN_VIN_buf3, OBD_CAN_VIN_buf4, OBD_CAN_VIN_buf5, OBD_CAN_VIN_buf6, OBD_CAN_VIN_buf7, OBD_CAN_VIN_buf8;
extern String OBD_CAN_VIN_buf9, OBD_CAN_VIN_buf10, OBD_CAN_VIN_buf11, OBD_CAN_VIN_buf12, OBD_CAN_VIN_buf13, OBD_CAN_VIN_buf14, OBD_CAN_VIN_buf15, OBD_CAN_VIN_buf16, OBD_CAN_VIN_buf17;
 
extern String OBD_CAN_CALID_buf1, OBD_CAN_CALID_buf2, OBD_CAN_CALID_buf3, OBD_CAN_CALID_buf4, OBD_CAN_CALID_buf5, OBD_CAN_CALID_buf6, OBD_CAN_CALID_buf7, OBD_CAN_CALID_buf8;
extern String OBD_CAN_CALID_buf9, OBD_CAN_CALID_buf10, OBD_CAN_CALID_buf11, OBD_CAN_CALID_buf12, OBD_CAN_CALID_buf13, OBD_CAN_CALID_buf14, OBD_CAN_CALID_buf15, OBD_CAN_CALID_buf16, OBD_CAN_CALID_buf17;
 
extern String OBD_CAN_CAL_buf1, OBD_CAN_CAL_buf2, OBD_CAN_CAL_buf3, OBD_CAN_CAL_buf4, OBD_CAN_CAL_buf5, OBD_CAN_CAL_buf6, OBD_CAN_CAL_buf7, OBD_CAN_CAL_buf8, OBD_CAN_CAL_buf9, OBD_CAN_CAL_buf10;
extern String OBD_CAN_CAL_buf11,CAL_buf12, OBD_CAN_CAL_buf13, OBD_CAN_CAL_buf14, OBD_CAN_CAL_buf15, OBD_CAN_CAL_buf16, OBD_CAN_CAL_buf17;  // OBD_CAN_CAL is maximum 16 ASCC characters.
 
extern String OBD_CAN_ECUName_buf1, OBD_CAN_ECUName_buf2, OBD_CAN_ECUName_buf3, OBD_CAN_ECUName_buf4, OBD_CAN_ECUName_buf5, OBD_CAN_ECUName_buf6, OBD_CAN_ECUName_buf7, OBD_CAN_ECUName_buf8;
extern String OBD_CAN_ECUName_buf9, OBD_CAN_ECUName_buf10, OBD_CAN_ECUName_buf11, OBD_CAN_ECUName_buf12, OBD_CAN_ECUName_buf13, OBD_CAN_ECUName_buf14, OBD_CAN_ECUName_buf15, OBD_CAN_ECUName_buf16, OBD_CAN_ECUName_buf17;
extern String OBD_CAN_ECUName_buf18, OBD_CAN_ECUName_buf19, OBD_CAN_ECUName_buf20, OBD_CAN_ECUName_buf21, OBD_CAN_ECUName_buf22, OBD_CAN_ECUName_buf23, OBD_CAN_ECUName_buf24, OBD_CAN_ECUName_buf25, OBD_CAN_ECUName_buf26, OBD_CAN_ECUName_buf27;


extern unsigned int OBD_CAN_msg0x77A_CF20_buf0, OBD_CAN_msg0x77A_CF20_buf1, OBD_CAN_msg0x77A_CF20_buf2, OBD_CAN_msg0x77A_CF20_buf3, OBD_CAN_msg0x77A_CF20_buf4, OBD_CAN_msg0x77A_CF20_buf5, OBD_CAN_msg0x77A_CF20_buf6, OBD_CAN_msg0x77A_CF20_buf7;  // 0x77A is Multi-Function Steering Wheel
extern unsigned int OBD_CAN_msg0x77A_CF21_buf0, OBD_CAN_msg0x77A_CF21_buf1, OBD_CAN_msg0x77A_CF21_buf2, OBD_CAN_msg0x77A_CF21_buf3, OBD_CAN_msg0x77A_CF21_buf4, OBD_CAN_msg0x77A_CF21_buf5, OBD_CAN_msg0x77A_CF21_buf6, OBD_CAN_msg0x77A_CF21_buf7;  // 0x77A is Multi-Function Steering Wheel

//  Raw buffer variables from OBD_CAN_Sniffing - to be used translated later in processing functions ------------------------------------------------------------------------

// ECU 0x700 CAN Frame buffer variables from OBD_CAN Sniffing
extern word OBD_CAN_msg0x70_Raw, OBD_CAN_msg0x71_Raw, OBD_CAN_msg0x72_Raw, OBD_CAN_msg0x73_Raw, OBD_CAN_msg0x74_Raw, OBD_CAN_msg0x75_Raw, OBD_CAN_msg0x76_Raw, OBD_CAN_msg0x77_Raw;

// ECU 0x7E8 CAN Frame buffer variables from OBD_CAN Sniffing
extern word OBD_CAN_msg0x7E8_buf0_Raw, OBD_CAN_msg0x7E8_buf1_Raw, OBD_CAN_msg0x7E8_buf2_Raw, OBD_CAN_msg0x7E8_buf3_Raw, OBD_CAN_msg0x7E8_buf4_Raw, OBD_CAN_msg0x7E8_buf5_Raw, OBD_CAN_msg0x7E8_buf6_Raw, OBD_CAN_msg0x7E8_buf7_Raw;

// ECU 0x7E8 CruiseControl Stalk
extern word OBD_CAN_msg0x7E8_ccStalk_buf0_Raw, OBD_CAN_msg0x7E8_ccStalk_buf1_Raw, OBD_CAN_msg0x7E8_ccStalk_buf2_Raw, OBD_CAN_msg0x7E8_ccStalk_buf3_Raw, OBD_CAN_msg0x7E8_ccStalk_buf4_Raw, OBD_CAN_msg0x7E8_ccStalk_buf5_Raw, OBD_CAN_msg0x7E8_ccStalk_buf6_Raw, OBD_CAN_msg0x7E8_ccStalk_buf7_Raw;

// TCU 0x7E9 CAN Frame buffer variables from OBD_CAN Sniffing
extern word OBD_CAN_msg0x7E9_buf0_Raw, OBD_CAN_msg0x7E9_buf1_Raw, OBD_CAN_msg0x7E9_buf2_Raw, OBD_CAN_msg0x7E9_buf3_Raw, OBD_CAN_msg0x7E9_buf4_Raw, OBD_CAN_msg0x7E9_buf5_Raw, OBD_CAN_msg0x7E9_buf6_Raw, OBD_CAN_msg0x7E9_buf7_Raw;
extern word OBD_CAN_msg0x7E9_x1buf0_Raw, OBD_CAN_msg0x7E9_x1buf1_Raw, OBD_CAN_msg0x7E9_x1buf2_Raw, OBD_CAN_msg0x7E9_x1buf3_Raw, OBD_CAN_msg0x7E9_x1buf4_Raw, OBD_CAN_msg0x7E9_x1buf5_Raw, OBD_CAN_msg0x7E9_x1buf6_Raw, OBD_CAN_msg0x7E9_x1buf7_Raw;
extern word OBD_CAN_msg0x7E9_x21buf0_Raw, OBD_CAN_msg0x7E9_x21buf1_Raw, OBD_CAN_msg0x7E9_x21buf2_Raw, OBD_CAN_msg0x7E9_x21buf3_Raw, OBD_CAN_msg0x7E9_x21buf4_Raw, OBD_CAN_msg0x7E9_x21buf5_Raw, OBD_CAN_msg0x7E9_x21buf6_Raw, OBD_CAN_msg0x7E9_x21buf7_Raw;
extern word OBD_CAN_msg0x7E9_x22buf0_Raw, OBD_CAN_msg0x7E9_x22buf1_Raw, OBD_CAN_msg0x7E9_x22buf2_Raw, OBD_CAN_msg0x7E9_x22buf3_Raw, OBD_CAN_msg0x7E9_x22buf4_Raw, OBD_CAN_msg0x7E9_x22buf5_Raw, OBD_CAN_msg0x7E9_x22buf6_Raw, OBD_CAN_msg0x7E9_x22buf7_Raw;
extern word OBD_CAN_msg0x7E9_x23buf0_Raw, OBD_CAN_msg0x7E9_x23buf1_Raw, OBD_CAN_msg0x7E9_x23buf2_Raw, OBD_CAN_msg0x7E9_x23buf3_Raw, OBD_CAN_msg0x7E9_x23buf4_Raw, OBD_CAN_msg0x7E9_x23buf5_Raw, OBD_CAN_msg0x7E9_x23buf6_Raw, OBD_CAN_msg0x7E9_x23buf7_Raw;
extern word OBD_CAN_msg0x7E9_x24buf0_Raw, OBD_CAN_msg0x7E9_x24buf1_Raw, OBD_CAN_msg0x7E9_x24buf2_Raw, OBD_CAN_msg0x7E9_x24buf3_Raw, OBD_CAN_msg0x7E9_x24buf4_Raw, OBD_CAN_msg0x7E9_x24buf5_Raw, OBD_CAN_msg0x7E9_x24buf6_Raw, OBD_CAN_msg0x7E9_x24buf7_Raw;


// TCU 0x7E9 CAN Frame buffer variables from PT_CAN Sniffing
extern word PT_CAN_msg0x7E9_buf0_Raw, PT_CAN_msg0x7E9_buf1_Raw, PT_CAN_msg0x7E9_buf2_Raw, PT_CAN_msg0x7E9_buf3_Raw, PT_CAN_msg0x7E9_buf4_Raw, PT_CAN_msg0x7E9_buf5_Raw, PT_CAN_msg0x7E9_buf6_Raw, PT_CAN_msg0x7E9_buf7_Raw;
extern word PT_CAN_msg0x7E9_x1buf0_Raw, PT_CAN_msg0x7E9_x1buf1_Raw, PT_CAN_msg0x7E9_x1buf2_Raw, PT_CAN_msg0x7E9_x1buf3_Raw, PT_CAN_msg0x7E9_x1buf4_Raw, PT_CAN_msg0x7E9_x1buf5_Raw, PT_CAN_msg0x7E9_x1buf6_Raw, PT_CAN_msg0x7E9_x1buf7_Raw;
extern word PT_CAN_msg0x7E9_x21buf0_Raw, PT_CAN_msg0x7E9_x21buf1_Raw, PT_CAN_msg0x7E9_x21buf2_Raw, PT_CAN_msg0x7E9_x21buf3_Raw, PT_CAN_msg0x7E9_x21buf4_Raw, PT_CAN_msg0x7E9_x21buf5_Raw, PT_CAN_msg0x7E9_x21buf6_Raw, PT_CAN_msg0x7E9_x21buf7_Raw;
extern word PT_CAN_msg0x7E9_x22buf0_Raw, PT_CAN_msg0x7E9_x22buf1_Raw, PT_CAN_msg0x7E9_x22buf2_Raw, PT_CAN_msg0x7E9_x22buf3_Raw, PT_CAN_msg0x7E9_x22buf4_Raw, PT_CAN_msg0x7E9_x22buf5_Raw, PT_CAN_msg0x7E9_x22buf6_Raw, PT_CAN_msg0x7E9_x22buf7_Raw;
extern word PT_CAN_msg0x7E9_x23buf0_Raw, PT_CAN_msg0x7E9_x23buf1_Raw, PT_CAN_msg0x7E9_x23buf2_Raw, PT_CAN_msg0x7E9_x23buf3_Raw, PT_CAN_msg0x7E9_x23buf4_Raw, PT_CAN_msg0x7E9_x23buf5_Raw, PT_CAN_msg0x7E9_x23buf6_Raw, PT_CAN_msg0x7E9_x23buf7_Raw;
extern word PT_CAN_msg0x7E9_x24buf0_Raw, PT_CAN_msg0x7E9_x24buf1_Raw, PT_CAN_msg0x7E9_x24buf2_Raw, PT_CAN_msg0x7E9_x24buf3_Raw, PT_CAN_msg0x7E9_x24buf4_Raw, PT_CAN_msg0x7E9_x24buf5_Raw, PT_CAN_msg0x7E9_x24buf6_Raw, PT_CAN_msg0x7E9_x24buf7_Raw;

// MFSW 0x77A CAN Frame buffer variables from OBD CAN Sniffing (via Service22 Requests)
extern word OBD_CAN_msg0x77A_buf0_Raw, OBD_CAN_msg0x77A_buf1_Raw, OBD_CAN_msg0x77A_buf2_Raw, OBD_CAN_msg0x77A_buf3_Raw, OBD_CAN_msg0x77A_buf4_Raw, OBD_CAN_msg0x77A_buf5_Raw, OBD_CAN_msg0x77A_buf6_Raw, OBD_CAN_msg0x77A_buf7_Raw;

// AWD 0x77D CAN Frame buffer variables from OBD CAN Sniffing (via Service22 Requests)
extern word OBD_CAN_msg0x77D_buf0_Raw, OBD_CAN_msg0x77D_buf1_Raw, OBD_CAN_msg0x77D_buf2_Raw, OBD_CAN_msg0x77D_buf3_Raw, OBD_CAN_msg0x77D_buf4_Raw, OBD_CAN_msg0x77D_buf5_Raw, OBD_CAN_msg0x77D_buf6_Raw, OBD_CAN_msg0x77D_buf7_Raw;

// Susp/Elect. 0x7DC CAN Frame buffer variables from OBD CAN Sniffing (via Service22 Requests)
extern word OBD_CAN_msg0x7DC_buf0_Raw, OBD_CAN_msg0x7DC_buf1_Raw, OBD_CAN_msg0x7DC_buf2_Raw, OBD_CAN_msg0x7DC_buf3_Raw, OBD_CAN_msg0x7DC_buf4_Raw, OBD_CAN_msg0x7DC_buf5_Raw, OBD_CAN_msg0x7DC_buf6_Raw, OBD_CAN_msg0x7DC_buf7_Raw;


//  Raw buffer variables from PT_CAN_Sniffing - to be used translated later in processing functions ------------------------------------------------------------------------

extern word PT_CAN_msg0x40_buf0_Raw, PT_CAN_msg0x40_buf1_Raw, PT_CAN_msg0x40_buf2_Raw, PT_CAN_msg0x40_buf3_Raw, PT_CAN_msg0x40_buf4_Raw, PT_CAN_msg0x40_buf5_Raw, PT_CAN_msg0x40_buf6_Raw, PT_CAN_msg0x40_buf7_Raw;
extern word PT_CAN_msg0xA8_buf0_Raw, PT_CAN_msg0xA8_buf1_Raw, PT_CAN_msg0xA8_buf2_Raw, PT_CAN_msg0xA8_buf3_Raw, PT_CAN_msg0xA8_buf4_Raw, PT_CAN_msg0xA8_buf5_Raw, PT_CAN_msg0xA8_buf6_Raw, PT_CAN_msg0xA8_buf7_Raw;
extern word PT_CAN_msg0xAD_buf0_Raw, PT_CAN_msg0xAD_buf1_Raw, PT_CAN_msg0xAD_buf2_Raw, PT_CAN_msg0xAD_buf3_Raw, PT_CAN_msg0xAD_buf4_Raw, PT_CAN_msg0xAD_buf5_Raw, PT_CAN_msg0xAD_buf6_Raw, PT_CAN_msg0xAD_buf7_Raw;
extern word PT_CAN_msg0xB2_buf0_Raw, PT_CAN_msg0xB2_buf1_Raw, PT_CAN_msg0xB2_buf2_Raw, PT_CAN_msg0xB2_buf3_Raw, PT_CAN_msg0xB2_buf4_Raw, PT_CAN_msg0xB2_buf5_Raw, PT_CAN_msg0xB2_buf6_Raw, PT_CAN_msg0xB2_buf7_Raw;
extern word PT_CAN_msg0x86_buf0_Raw, PT_CAN_msg0x86_buf1_Raw, PT_CAN_msg0x86_buf2_Raw, PT_CAN_msg0x86_buf3_Raw, PT_CAN_msg0x86_buf4_Raw, PT_CAN_msg0x86_buf5_Raw, PT_CAN_msg0x86_buf6_Raw, PT_CAN_msg0x86_buf7_Raw;
extern word PT_CAN_msg0x104_buf0_Raw, PT_CAN_msg0x104_buf1_Raw, PT_CAN_msg0x104_buf2_Raw, PT_CAN_msg0x104_buf3_Raw, PT_CAN_msg0x104_buf4_Raw, PT_CAN_msg0x104_buf5_Raw, PT_CAN_msg0x104_buf6_Raw, PT_CAN_msg0x104_buf7_Raw;
extern word PT_CAN_msg0x106_buf0_Raw, PT_CAN_msg0x106_buf1_Raw, PT_CAN_msg0x106_buf2_Raw, PT_CAN_msg0x106_buf3_Raw, PT_CAN_msg0x106_buf4_Raw, PT_CAN_msg0x106_buf5_Raw, PT_CAN_msg0x106_buf6_Raw, PT_CAN_msg0x106_buf7_Raw;
extern word PT_CAN_msg0x106_AIMtx_buf0_Raw, PT_CAN_msg0x106_AIMtx_buf1_Raw, PT_CAN_msg0x106_AIMtx_buf2_Raw, PT_CAN_msg0x106_AIMtx_buf3_Raw, PT_CAN_msg0x106_AIMtx_buf4_Raw, PT_CAN_msg0x106_AIMtx_buf5_Raw, PT_CAN_msg0x106_AIMtx_buf6_Raw, PT_CAN_msg0x106_AIMtx_buf7_Raw;
extern word PT_CAN_msg0x107_buf0_Raw, PT_CAN_msg0x107_buf1_Raw, PT_CAN_msg0x107_buf2_Raw, PT_CAN_msg0x107_buf3_Raw, PT_CAN_msg0x107_buf4_Raw, PT_CAN_msg0x107_buf5_Raw, PT_CAN_msg0x107_buf6_Raw, PT_CAN_msg0x107_buf7_Raw;
extern word PT_CAN_msg0x120_buf0_Raw, PT_CAN_msg0x120_buf1_Raw, PT_CAN_msg0x120_buf2_Raw, PT_CAN_msg0x120_buf3_Raw, PT_CAN_msg0x120_buf4_Raw, PT_CAN_msg0x120_buf5_Raw, PT_CAN_msg0x120_buf6_Raw, PT_CAN_msg0x120_buf7_Raw;
extern word PT_CAN_msg0x12B_buf0_Raw, PT_CAN_msg0x12B_buf1_Raw, PT_CAN_msg0x12B_buf2_Raw, PT_CAN_msg0x12B_buf3_Raw, PT_CAN_msg0x12B_buf4_Raw, PT_CAN_msg0x12B_buf5_Raw, PT_CAN_msg0x12B_buf6_Raw, PT_CAN_msg0x12B_buf7_Raw;
extern word PT_CAN_msg0x147_buf0_Raw, PT_CAN_msg0x147_buf1_Raw, PT_CAN_msg0x147_buf2_Raw, PT_CAN_msg0x147_buf3_Raw, PT_CAN_msg0x147_buf4_Raw, PT_CAN_msg0x147_buf5_Raw, PT_CAN_msg0x147_buf6_Raw, PT_CAN_msg0x147_buf7_Raw;
extern word PT_CAN_msg0x1AB_buf0_Raw, PT_CAN_msg0x1AB_buf1_Raw, PT_CAN_msg0x1AB_buf2_Raw, PT_CAN_msg0x1AB_buf3_Raw, PT_CAN_msg0x1AB_buf4_Raw, PT_CAN_msg0x1AB_buf5_Raw, PT_CAN_msg0x1AB_buf6_Raw, PT_CAN_msg0x1AB_buf7_Raw;
extern word PT_CAN_msg0x385_buf0_Raw, PT_CAN_msg0x385_buf1_Raw, PT_CAN_msg0x385_buf2_Raw, PT_CAN_msg0x385_buf3_Raw, PT_CAN_msg0x385_buf4_Raw, PT_CAN_msg0x385_buf5_Raw, PT_CAN_msg0x385_buf6_Raw, PT_CAN_msg0x385_buf7_Raw;
extern word PT_CAN_msg0x31B_buf0_Raw, PT_CAN_msg0x31B_buf1_Raw, PT_CAN_msg0x31B_buf2_Raw, PT_CAN_msg0x31B_buf3_Raw, PT_CAN_msg0x31B_buf4_Raw, PT_CAN_msg0x31B_buf5_Raw, PT_CAN_msg0x31B_buf6_Raw, PT_CAN_msg0x31B_buf7_Raw;
extern word PT_CAN_msg0x391_buf0_Raw, PT_CAN_msg0x391_buf1_Raw, PT_CAN_msg0x391_buf2_Raw, PT_CAN_msg0x391_buf3_Raw, PT_CAN_msg0x391_buf4_Raw, PT_CAN_msg0x391_buf5_Raw, PT_CAN_msg0x391_buf6_Raw, PT_CAN_msg0x391_buf7_Raw;
extern word PT_CAN_msg0x394_buf0_Raw, PT_CAN_msg0x394_buf1_Raw, PT_CAN_msg0x394_buf2_Raw, PT_CAN_msg0x394_buf3_Raw, PT_CAN_msg0x394_buf4_Raw, PT_CAN_msg0x394_buf5_Raw, PT_CAN_msg0x394_buf6_Raw, PT_CAN_msg0x394_buf7_Raw;
extern word PT_CAN_msg0x3DD_buf0_Raw, PT_CAN_msg0x3DD_buf1_Raw, PT_CAN_msg0x3DD_buf2_Raw, PT_CAN_msg0x3DD_buf3_Raw, PT_CAN_msg0x3DD_buf4_Raw, PT_CAN_msg0x3DD_buf5_Raw, PT_CAN_msg0x3DD_buf6_Raw, PT_CAN_msg0x3DD_buf7_Raw; // 0x3DD is for paddle positions/shift status
extern word PT_CAN_msg0x743_buf0_Raw, PT_CAN_msg0x743_buf1_Raw, PT_CAN_msg0x743_buf2_Raw, PT_CAN_msg0x743_buf3_Raw, PT_CAN_msg0x743_buf4_Raw, PT_CAN_msg0x743_buf5_Raw, PT_CAN_msg0x743_buf6_Raw, PT_CAN_msg0x743_buf7_Raw;
extern word PT_CAN_msg0x7EE_buf0_Raw, PT_CAN_msg0x7EE_buf1_Raw, PT_CAN_msg0x7EE_buf2_Raw, PT_CAN_msg0x7EE_buf3_Raw, PT_CAN_msg0x7EE_buf4_Raw, PT_CAN_msg0x7EE_buf5_Raw, PT_CAN_msg0x7EE_buf6_Raw, PT_CAN_msg0x7EE_buf7_Raw;

extern unsigned int PT_CAN_msg0x40_buf0, PT_CAN_msg0x40_buf1, PT_CAN_msg0x40_buf2, PT_CAN_msg0x40_buf3, PT_CAN_msg0x40_buf4, PT_CAN_msg0x40_buf5, PT_CAN_msg0x40_buf6, PT_CAN_msg0x40_buf7;
extern unsigned int PT_CAN_msg0x120_buf0, PT_CAN_msg0x120_buf1, PT_CAN_msg0x120_buf2, PT_CAN_msg0x120_buf3, PT_CAN_msg0x120_buf4, PT_CAN_msg0x120_buf5, PT_CAN_msg0x120_buf6, PT_CAN_msg0x120_buf7;
extern unsigned int PT_CAN_msg0x1AB_buf0, PT_CAN_msg0x1AB_buf1, PT_CAN_msg0x1AB_buf2, PT_CAN_msg0x1AB_buf3, PT_CAN_msg0x1AB_buf4, PT_CAN_msg0x1AB_buf5, PT_CAN_msg0x1AB_buf6, PT_CAN_msg0x1AB_buf7;
extern unsigned int PT_CAN_msg0x31B_buf0, PT_CAN_msg0x31B_buf1, PT_CAN_msg0x31B_buf2, PT_CAN_msg0x31B_buf3, PT_CAN_msg0x31B_buf4, PT_CAN_msg0x31B_buf5, PT_CAN_msg0x31B_buf6, PT_CAN_msg0x31B_buf7;
extern unsigned int PT_CAN_msg0x391_buf0, PT_CAN_msg0x391_buf1, PT_CAN_msg0x391_buf2, PT_CAN_msg0x391_buf3, PT_CAN_msg0x391_buf4, PT_CAN_msg0x391_buf5, PT_CAN_msg0x391_buf6, PT_CAN_msg0x391_buf7;
extern unsigned int PT_CAN_msg0x3DD_buf0, PT_CAN_msg0x3DD_buf1, PT_CAN_msg0x3DD_buf2, PT_CAN_msg0x3DD_buf3, PT_CAN_msg0x3DD_buf4, PT_CAN_msg0x3DD_buf5, PT_CAN_msg0x3DD_buf6, PT_CAN_msg0x3DD_buf7;



//  Raw buffer variables from CAN_Sniffing - to be used translated later in processing functions ------------------------------------------------------------------------




#pragma region // FlexCAN Timers for PT_CAN and OBD_CAN

// PT_CAN Timers

extern elapsedMillis GaugeTimer1;
extern elapsedMillis GaugeTimer2;
extern elapsedMillis GaugeTimer3;
extern elapsedMillis GaugeTimer4;
extern elapsedMillis IMUCalibrationDelay;
extern elapsedMillis Susp_LvlSnsr_Offsets_Reset_Delay;
extern elapsedMillis loopDelayLevelSensors;


extern elapsedMillis TimeSinceSTATFC1;
extern elapsedMillis TimeSinceSTATFC2;

extern elapsedMillis PT_CAN_canTxInterval_0x780;
extern elapsedMillis PT_CAN_canTxInterval_0x781;
extern elapsedMillis PT_CAN_canTxInterval_0x782;
extern elapsedMillis PT_CAN_canTxInterval_0x783;
extern elapsedMillis PT_CAN_canTxInterval_0x784;
extern elapsedMillis PT_CAN_canTxInterval_0x785;
extern elapsedMillis PT_CAN_canTxInterval_0x786_mux1;
extern elapsedMillis PT_CAN_canTxInterval_0x786_mux2;
extern elapsedMillis PT_CAN_canTxInterval_0x786_mux3;
extern elapsedMillis PT_CAN_canTxInterval_0x787_mux1;
extern elapsedMillis PT_CAN_canTxInterval_0x787_mux2;
extern elapsedMillis PT_CAN_canTxInterval_0x787_mux3;
extern elapsedMillis PT_CAN_canTxInterval_0x788;
extern elapsedMillis PT_CAN_canTxInterval_0x789;


extern elapsedMillis PT_CAN_canTxInterval_0x791;
extern elapsedMillis PT_CAN_canTxInterval_0x792;
extern elapsedMillis PT_CAN_canTxInterval_0x793;
extern elapsedMillis PT_CAN_canTxInterval_0x794;
extern elapsedMillis PT_CAN_canTxInterval_0x795;
extern elapsedMillis PT_CAN_canTxInterval_0x796;


extern elapsedMillis PT_CAN_canTxInterval_0x786_forAIM;
extern elapsedMillis PT_CAN_canTxInterval_0x787_forAIM;
extern elapsedMillis PT_CAN_canTxInterval_0x789_forAIM;
extern elapsedMillis PT_CAN_canTxInterval_0x790_forAIM;
extern elapsedMillis PT_CAN_canTxInterval_0x791_forAIM;
extern elapsedMillis PT_CAN_canTxInterval_0x792_forAIM;
extern elapsedMillis PT_CAN_canTxInterval_0x793_forAIM;




extern elapsedMicros PT_CAN_canTxInterval_0x147; // DisplayGauges Override, in Micros
extern elapsedMicros PT_CAN_canTxInterval_0x107; // Tachometer Override, in Micros

extern elapsedMillis OBD_CAN_canTxInterval_0x794;

extern elapsedMillis TimeSinceWhlSpd0;
extern elapsedMillis loopDelayAutoShift_RPMSetpoint_ChangeRPM;
extern elapsedMillis loopDelayAutoUPshift_RPMSetpoint_SetRPM;
 
extern elapsedMillis loopDelayAutoShift_GearShiftToUpdate;
extern elapsedMillis loopDelayTFTConfig_VariableUpdate;

extern elapsedMillis MQB_LaunchControl_EngineRPM_RAMP_Mode_Timer;


extern elapsedMillis loopDelayOBD_Service09;
extern elapsedMillis loopDelayOBD_Service22_MFSW;
extern elapsedMillis loopDelayOBD_Service22_MFSW_HungFC;

extern elapsedMillis PaddleUP_SignalDebounceDelay;
extern elapsedMillis PaddleDOWN_SignalDebounceDelay;


extern elapsedMillis CruiseStalk_RES_Timer;
extern elapsedMillis CruiseStalk_FORWARD_Timer;
extern elapsedMillis CruiseStalk_UP_Timer;
extern elapsedMillis CruiseStalk_DOWN_Timer;
extern elapsedMillis CruiseStalk_SET_Timer;
extern elapsedMillis ConfigurationUpdateMode1_TimeoutTimer;

extern elapsedMillis CruiseStalk_RES_DebounceTimer;
extern elapsedMillis CruiseStalk_FORWARD_DebounceTimer;
extern elapsedMillis CruiseStalk_UP_DebounceTimer;
extern elapsedMillis CruiseStalk_DOWN_DebounceTimer;
extern elapsedMillis CruiseStalk_SET_DebounceTimer;



extern elapsedMillis loopDelayPT_CAN_DTC_Clear;
extern elapsedMillis OBD_DTC_Clear_Delay;
extern elapsedMillis loopDelayOBD_DTC_Clear;


extern elapsedMillis loopDelay_PT_CAN_LPFP_Activation;
extern elapsedMillis loopDelay_PT_CAN_LPFP_DEActivation;

extern elapsedMillis PT_CAN_LPFP_Activation_TimeSinceSignal;
extern elapsedMillis PT_CAN_LPFP_DEActivation_TimeSinceSignal;
extern elapsedMillis PT_CAN_LPFP_Activation_Continuous_FirstMessage_TimeSinceSignal;
extern elapsedMillis PT_CAN_LPFP_Activation_Continuous_SecondMessage_TimeSinceSignal;

extern elapsedMillis loopDelay_OBD_DynoMode;
extern elapsedMillis loopDelay_OBD_DynoModeActivation_Continous;
extern elapsedMillis loopDelay_OBD_DynoModeDE_Activation_Continous;


extern elapsedMillis loopDelayOBD_CAN_TCU_ReqSet1_ClutchPressureActuals; // Old Set 1 - Pressure Actuals
extern elapsedMillis loopDelayPT_CAN_TCU_ReqSet1_ClutchPressureActuals; // Old Set 1 - Pressure Actuals

extern elapsedMillis TimeSinceSTATFC2_OBD_CAN_OLD_TCU_ReqSet1;  // Old Set 1 - Pressure Actuals
extern elapsedMillis TimeSinceSTATFC1_PT_CAN_OLD_TCU_ReqSet1;  // Old Set 1 - Pressure Actuals
extern elapsedMillis TimeSinceFirstResponseRecieved_TCU_Set1;  // Old Set 1 - Pressure Actuals

extern elapsedMillis loopDelayOBD_CAN_TCU_AllTqsAndPressures_Tx_1;
extern elapsedMillis loopDelayOBD_CAN_TCU_AllTqsAndPressures_Tx_2;
extern elapsedMillis loopDelayOBD_CAN_TCU_AllTqsAndPressures_Tx_3;
extern elapsedMillis loopDelayOBD_CAN_TCU_AllTqsAndPressures_Tx_4;

extern elapsedMillis loopDelayPT_CAN_TCU_AllTqsAndPressures_Tx_1;
extern elapsedMillis loopDelayPT_CAN_TCU_AllTqsAndPressures_Tx_2;
extern elapsedMillis loopDelayPT_CAN_TCU_AllTqsAndPressures_Tx_3;
extern elapsedMillis loopDelayPT_CAN_TCU_AllTqsAndPressures_Tx_4;



extern elapsedMillis OBD_CAN_TimeSinceSTATFC1_AllTqsAndPressures;
extern elapsedMillis TimeSinceSTATFC2_OBD_CAN_AllTqsAndPressures;
extern elapsedMillis TimeSinceSTATFC3_OBD_CAN_AllTqsAndPressures;
extern elapsedMillis TimeSinceFinalFCSent_OBD_CAN_AllTqsAndPressures;

extern elapsedMillis PT_CAN_TimeSinceSTATFC1_AllTqsAndPressures;
extern elapsedMillis TimeSinceSTATFC2_PT_CAN_AllTqsAndPressures;
extern elapsedMillis TimeSinceSTATFC3_PT_CAN_AllTqsAndPressures;
extern elapsedMillis TimeSinceFinalFCSent_PT_CAN_AllTqsAndPressures;




extern elapsedMillis loopDelayTCU_Set3Temps_Tx_1;
extern elapsedMillis loopDelayTCU_Set3Temps_Tx_2;
extern elapsedMillis loopDelayTCU_Set3Temps_Tx_3;
extern elapsedMillis loopDelayTCU_Set3Temps_Tx_4;


extern elapsedMillis TimeSinceSTATFC1_Set3Temps;
extern elapsedMillis TimeSinceSTATFC2_Set3Temps;
extern elapsedMillis TimeSinceInitialReq_OBD_CAN_TCUSet3Temps;
extern elapsedMillis TimesinceFCFinalSent_OBD_CAN_TCUSet3Temps;



extern elapsedMillis loopDelayConfigurationUpdate; 
extern elapsedMillis loopDelayConfigurationModeEntry;
extern elapsedMillis loopDelayConfigurationUpdate_JobUpdate;

extern elapsedMillis loopDelayOBD_ConfigurationUpdate;
extern elapsedMillis loopDelayPT_CAN_ConfigurationUpdate;

extern elapsedMillis loopDelayOBD_KeepAwake_0x700;
extern elapsedMillis loopDelayPT_CAN_KeepAwake_0x700;

extern elapsedMillis DS1_ECU_LoggingConfigActive; 
extern int DS1_ECU_LoggingConfigActive_Int;


extern word IgnitionStatusTimerCAN;
extern elapsedMillis IgnitionStatusTimer_TimeSinceLastMessage;
extern elapsedMillis IgnitionStatusTimer_TimeSinceIgnitionON;

// extern elapsedMillis loopTimer_20ms;


// OBD Timers


extern elapsedMillis loopDelayDynoModeRead;
extern elapsedMillis loopDelayDynoModeActivation;
extern elapsedMillis loopDelayDynoModeDEActivation;


extern elapsedMillis OBD_DynoModeRead_TimeSinceSignal;
extern elapsedMillis OBD_DynoModeActivation_TimeSinceSignal;
extern elapsedMillis OBD_DynoModeDEActivation_TimeSinceSignal;
extern elapsedMillis OBD_DynoMod_ReadStatuses_Consolidated_Timer;
 
 
extern elapsedMillis OBD_CAN_msg0x7E9_WakeupResponse_TimeSinceResponse;
extern elapsedMillis OBD_CAN_msg0x7E8_WakeupResponse_TimeSinceResponse;

extern elapsedMillis OBD_CAN_msg0x7C1_WakeupResponse_TimeSinceResponse;
extern elapsedMillis OBD_CAN_msg0x7B9_WakeupResponse_TimeSinceResponse;
extern elapsedMillis OBD_CAN_msg0x7B8_WakeupResponse_TimeSinceResponse;
extern elapsedMillis OBD_CAN_msg0x77E_WakeupResponse_TimeSinceResponse;
extern elapsedMillis OBD_CAN_msg0x77D_WakeupResponse_TimeSinceResponse;
extern elapsedMillis OBD_CAN_msg0x779_WakeupResponse_TimeSinceResponse;
extern elapsedMillis OBD_CAN_msg0x77A_WakeupResponse_TimeSinceResponse; // 0x77A is MFSW

extern elapsedMillis OBD_CAN_msg0x700_TimeSinceWakeupSignalSent;
extern elapsedMillis OBD_CAN_msg0x700_TimeSinceActivationSignalSent;
extern elapsedMillis OBD_CAN_msg0x700_TimeSinceDEActivationSignalSent;





#pragma endregion





extern unsigned int DisplayGaugeTVal;
extern unsigned int DisplayGaugePVal;

extern unsigned int DisplayGaugeTVal_Direct;
extern unsigned int DisplayGaugePVal_Direct;

extern unsigned int DisplayGaugeTVal_Direct_ConvInt;
extern unsigned int DisplayGaugePVal_Direct_ConvInt;

extern unsigned int DisplayTachOverride_Active;

// Driver Inputs as sniffed from PT-CAN
extern unsigned int AccelPedal;
extern unsigned int accelPedal2;
extern unsigned int AccelPedal_0x107;
extern unsigned int AccelPedal_0x391;
extern unsigned int AccelPedal_0x391_Mapped;
extern float AccelPedal_0x391_Mapped_Float;



extern unsigned int ThrottleTPS_0x391;

extern unsigned int PaddlePositions;
extern unsigned int BrakeSwitch_0x1AB;
extern unsigned int BrakePedalSwitchStatus;

extern word BrakePressure_0x106_CAN;
extern unsigned int BrakePressure_0x106_Raw;
extern unsigned int BrakePressureBAR_Int;
extern float BrakePressureBAR_Float;

extern word AccelPedal_0x391_AIMtx_CAN;
extern unsigned int AccelPedal_0x391_AIMtx_Raw;
extern unsigned int AccelPedal_0x391_AIMtx_Int;
extern unsigned int AccelPedal_0x391_AIMtx_Mapped;
extern float AccelPedal_0x391_AIMtx_Mapped_Float;


extern word BrakePressureBAR_0x106_AIMtx_CAN;
extern word BrakePressureBAR_0x106_AIMtx_Extracted;
extern unsigned int BrakePressureBAR_0x106_AIMtx_Raw;
extern unsigned int BrakePressureBAR_0x106_AIMtx_Int;
extern float BrakePressureBAR_0x106_AIMtx_Float;

extern  word BrakePressureBAR_MasterCylfromAIM_0x106_CAN;
extern  word BrakePressureBAR_MasterCylfromAIM_0x106_Extracted;
extern  unsigned int BrakePressureBAR_MasterCylfromAIM_0x106_Raw;
extern  unsigned int BrakePressureBAR_MasterCylfromAIM_Int;
extern  float BrakePressureBAR_MasterCylfromAIM_Float;

extern  word BrakePressureBAR_RearCylfromAIM_0x106_CAN;
extern  word BrakePressureBAR_RearCylfromAIM_0x106_Extracted;
extern  unsigned int BrakePressureBAR_RearCylfromAIM_0x106_Raw;
extern  unsigned int BrakePressureBAR_RearCylfromAIM_Int;
extern  float BrakePressureBAR_RearCylfromAIM_Float;




extern word MQB_SteeringWheelAngle_0x86_CAN;
extern word MQB_SteeringWheelAngle_0x86_Raw;
extern float MQB_SteeringWheelAngle_0x86_Float;
extern unsigned int MQB_SteeringWheelAngle_0x86_Int;
extern float MQB_SteeringWheelAngle_0x86_Radians_Int;



extern unsigned int HazardLightsButton;
extern unsigned int HazardLightsButton_0x366;

extern unsigned int GearLeverPosition_0xAD;
extern unsigned int GoalGear_0xAD;

extern unsigned int GearLeverPosition_Park;
extern unsigned int GearLeverPosition_Reverse;
extern unsigned int GearLeverPosition_Neutral;
extern unsigned int GearLeverPosition_Drive;
extern unsigned int GearLeverPosition_TIP;
 
extern unsigned int DQ500_GoalGear0xAD;
extern unsigned int DQ500_ClutchLockStatus;



extern unsigned int cruiseStalk;
extern unsigned int cruiseStalk_byte1;
extern unsigned int cruiseStalk_byte2;


extern unsigned int cruiseStalkStaticPosition_0x120;
extern unsigned int CruiseStalk_OFF;
extern unsigned int CruiseStalk_ON;
extern unsigned int CruiseStalk_UP;
extern unsigned int CruiseStalk_DOWN;
extern unsigned int CruiseStalk_RES;
extern unsigned int CruiseStalk_FORWARD;
extern unsigned int CruiseStalk_SET;
extern unsigned int CruiseStalk_DistanceUP;
extern unsigned int CruiseStalk_DistanceDOWN;

extern unsigned int cruiseStalkStaticPosition_0x120_bit;
extern unsigned int CruiseStalk_OFF_bit;
extern unsigned int CruiseStalk_ON_bit;
extern unsigned int CruiseStalk_UP_bit;
extern unsigned int CruiseStalk_DOWN_bit;
extern unsigned int CruiseStalk_RES_bit;
extern unsigned int CruiseStalk_FORWARD_bit;
extern unsigned int CruiseStalk_SET_bit;
extern unsigned int CruiseStalk_DistanceUP_bit;
extern unsigned int CruiseStalk_DistanceDOWN_bit;


extern unsigned int ConfigurationUpdateMode1;
extern unsigned int ConfigurationUpdateMode1_Active;

extern unsigned int ConfigurationUpdateMode_HaldexDirect;
extern unsigned int ConfigurationUpdateMode_HaldexDirect_Active;

extern unsigned int MQB_ConfigurationMode_JobNumber_to_Update;

extern unsigned int MQB_ConfigurationMode_JobNumber_to_Update_ConvertedForDisplay_Int;
extern unsigned int MQB_ConfigurationMode_JobNumber_to_Update_ConvertedForDisplay_DirectSUM;

extern unsigned int MQB_ConfigurationMode_Update_VariableSetting;
extern unsigned int MQB_ConfigurationMode_Update_VariableSetting_ConvertedForDisplay_Int;
extern unsigned int MQB_ConfigurationMode_Update_VariableSetting_ConvertedForDisplay_DirectSUM;
 
extern unsigned int ParkBrakeStatus;
extern unsigned int ParkBrakeStatus_0x104;

extern unsigned int Paddle_UP_Pulled;
extern unsigned int Paddle_DOWN_Pulled;
extern unsigned int BOTH_Paddles_Pulled; 



extern unsigned int EthanolPct;
extern unsigned int EthanolPctDS1;
extern unsigned int ParkBrakeStatus_0x104;

extern unsigned long TimeSinceIgnitionOff;

// extern word IgnitionStatusCAN;
extern unsigned int IgnitionStatus;
extern unsigned int IgnitionAwake;
extern unsigned int IgnitionStatusTimer;


#pragma endregion

#pragma region // FlexCAN Variables - Susp_CAN  (primarily used for passively sniffing SuspCAN variables of Operational and Driver Inputs)

#ifdef Concept_TFT_StandaloneHaldexController
extern FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> TFTCAN1;
#endif


extern word Susp_CAN_msg0x86_buf0_CAN, Susp_CAN_msg0x86_buf1_CAN, Susp_CAN_msg0x86_buf2_CAN, Susp_CAN_msg0x86_buf3_CAN, Susp_CAN_msg0x86_buf4_CAN, Susp_CAN_msg0x86_buf5_CAN, Susp_CAN_msg0x86_buf6_CAN, Susp_CAN_msg0x86_buf7_CAN;
extern word Susp_CAN_msg0xB2_buf0_CAN, Susp_CAN_msg0xB2_buf1_CAN, Susp_CAN_msg0xB2_buf2_CAN, Susp_CAN_msg0xB2_buf3_CAN, Susp_CAN_msg0xB2_buf4_CAN, Susp_CAN_msg0xB2_buf5_CAN, Susp_CAN_msg0xB2_buf6_CAN, Susp_CAN_msg0xB2_buf7_CAN;
extern word Susp_CAN_msg0x106_buf0_CAN, Susp_CAN_msg0x106_buf1_CAN, Susp_CAN_msg0x106_buf2_CAN, Susp_CAN_msg0x106_buf3_CAN, Susp_CAN_msg0x106_buf4_CAN, Susp_CAN_msg0x106_buf5_CAN, Susp_CAN_msg0x106_buf6_CAN, Susp_CAN_msg0x106_buf7_CAN;
extern word Susp_CAN_msg0x1AB_buf0_CAN, Susp_CAN_msg0x1AB_buf1_CAN, Susp_CAN_msg0x1AB_buf2_CAN, Susp_CAN_msg0x1AB_buf3_CAN, Susp_CAN_msg0x1AB_buf4_CAN, Susp_CAN_msg0x1AB_buf5_CAN, Susp_CAN_msg0x1AB_buf6_CAN, Susp_CAN_msg0x1AB_buf7_CAN;
extern word Susp_CAN_msg0x391_buf0_CAN, Susp_CAN_msg0x391_buf1_CAN, Susp_CAN_msg0x391_buf2_CAN, Susp_CAN_msg0x391_buf3_CAN, Susp_CAN_msg0x391_buf4_CAN, Susp_CAN_msg0x391_buf5_CAN, Susp_CAN_msg0x391_buf6_CAN, Susp_CAN_msg0x391_buf7_CAN;


extern word Susp_CAN_msg0x743_buf0_CAN, Susp_CAN_msg0x743_buf1_CAN, Susp_CAN_msg0x743_buf2_CAN, Susp_CAN_msg0x743_buf3_CAN, Susp_CAN_msg0x743_buf4_CAN, Susp_CAN_msg0x743_buf5_CAN, Susp_CAN_msg0x743_buf6_CAN, Susp_CAN_msg0x743_buf7_CAN;
extern word Susp_CAN_msg0x783_buf0_CAN, Susp_CAN_msg0x783_buf1_CAN, Susp_CAN_msg0x783_buf2_CAN, Susp_CAN_msg0x783_buf3_CAN, Susp_CAN_msg0x783_buf4_CAN, Susp_CAN_msg0x783_buf5_CAN, Susp_CAN_msg0x783_buf6_CAN, Susp_CAN_msg0x783_buf7_CAN;
extern word Susp_CAN_msg0x788_buf0_CAN, Susp_CAN_msg0x788_buf1_CAN, Susp_CAN_msg0x788_buf2_CAN, Susp_CAN_msg0x788_buf3_CAN, Susp_CAN_msg0x788_buf4_CAN, Susp_CAN_msg0x788_buf5_CAN, Susp_CAN_msg0x788_buf6_CAN, Susp_CAN_msg0x788_buf7_CAN;
extern word Susp_CAN_msg0x789_buf0_CAN, Susp_CAN_msg0x789_buf1_CAN, Susp_CAN_msg0x789_buf2_CAN, Susp_CAN_msg0x789_buf3_CAN, Susp_CAN_msg0x789_buf4_CAN, Susp_CAN_msg0x789_buf5_CAN, Susp_CAN_msg0x789_buf6_CAN, Susp_CAN_msg0x789_buf7_CAN;
extern word Susp_CAN_msg0x790_buf0_CAN, Susp_CAN_msg0x790_buf1_CAN, Susp_CAN_msg0x790_buf2_CAN, Susp_CAN_msg0x790_buf3_CAN, Susp_CAN_msg0x790_buf4_CAN, Susp_CAN_msg0x790_buf5_CAN, Susp_CAN_msg0x790_buf6_CAN, Susp_CAN_msg0x790_buf7_CAN;
extern word Susp_CAN_msg0x791_buf0_CAN, Susp_CAN_msg0x791_buf1_CAN, Susp_CAN_msg0x791_buf2_CAN, Susp_CAN_msg0x791_buf3_CAN, Susp_CAN_msg0x791_buf4_CAN, Susp_CAN_msg0x791_buf5_CAN, Susp_CAN_msg0x791_buf6_CAN, Susp_CAN_msg0x791_buf7_CAN;
extern word Susp_CAN_msg0x792_buf0_CAN, Susp_CAN_msg0x792_buf1_CAN, Susp_CAN_msg0x792_buf2_CAN, Susp_CAN_msg0x792_buf3_CAN, Susp_CAN_msg0x792_buf4_CAN, Susp_CAN_msg0x792_buf5_CAN, Susp_CAN_msg0x792_buf6_CAN, Susp_CAN_msg0x792_buf7_CAN;
extern word Susp_CAN_msg0x793_buf0_CAN, Susp_CAN_msg0x793_buf1_CAN, Susp_CAN_msg0x793_buf2_CAN, Susp_CAN_msg0x793_buf3_CAN, Susp_CAN_msg0x793_buf4_CAN, Susp_CAN_msg0x793_buf5_CAN, Susp_CAN_msg0x793_buf6_CAN, Susp_CAN_msg0x793_buf7_CAN;
extern word Susp_CAN_msg0x794_buf0_CAN, Susp_CAN_msg0x794_buf1_CAN, Susp_CAN_msg0x794_buf2_CAN, Susp_CAN_msg0x794_buf3_CAN, Susp_CAN_msg0x794_buf4_CAN, Susp_CAN_msg0x794_buf5_CAN, Susp_CAN_msg0x794_buf6_CAN, Susp_CAN_msg0x794_buf7_CAN;
extern word Susp_CAN_msg0x795_buf0_CAN, Susp_CAN_msg0x795_buf1_CAN, Susp_CAN_msg0x795_buf2_CAN, Susp_CAN_msg0x795_buf3_CAN, Susp_CAN_msg0x795_buf4_CAN, Susp_CAN_msg0x795_buf5_CAN, Susp_CAN_msg0x795_buf6_CAN, Susp_CAN_msg0x795_buf7_CAN;
extern word Susp_CAN_msg0x796_buf0_CAN, Susp_CAN_msg0x796_buf1_CAN, Susp_CAN_msg0x796_buf2_CAN, Susp_CAN_msg0x796_buf3_CAN, Susp_CAN_msg0x796_buf4_CAN, Susp_CAN_msg0x796_buf5_CAN, Susp_CAN_msg0x796_buf6_CAN, Susp_CAN_msg0x796_buf7_CAN;


extern unsigned int Susp_CAN_msg0xB2_buf0_Raw, Susp_CAN_msg0xB2_buf1_Raw, Susp_CAN_msg0xB2_buf2_Raw, Susp_CAN_msg0xB2_buf3_Raw, Susp_CAN_msg0xB2_buf4_Raw, Susp_CAN_msg0xB2_buf5_Raw, Susp_CAN_msg0xB2_buf6_Raw, Susp_CAN_msg0xB2_buf7_Raw;
extern unsigned int Susp_CAN_msg0x86_buf0_Raw, Susp_CAN_msg0x86_buf1_Raw, Susp_CAN_msg0x86_buf2_Raw, Susp_CAN_msg0x86_buf3_Raw, Susp_CAN_msg0x86_buf4_Raw, Susp_CAN_msg0x86_buf5_Raw, Susp_CAN_msg0x86_buf6_Raw, Susp_CAN_msg0x86_buf7_Raw;
extern unsigned int Susp_CAN_msg0x104_buf0_Raw, Susp_CAN_msg0x104_buf1_Raw, Susp_CAN_msg0x104_buf2_Raw, Susp_CAN_msg0x104_buf3_Raw, Susp_CAN_msg0x104_buf4_Raw, Susp_CAN_msg0x104_buf5_Raw, Susp_CAN_msg0x104_buf6_Raw, Susp_CAN_msg0x104_buf7_Raw;
extern unsigned int Susp_CAN_msg0x106_buf0_Raw, Susp_CAN_msg0x106_buf1_Raw, Susp_CAN_msg0x106_buf2_Raw, Susp_CAN_msg0x106_buf3_Raw, Susp_CAN_msg0x106_buf4_Raw, Susp_CAN_msg0x106_buf5_Raw, Susp_CAN_msg0x106_buf6_Raw, Susp_CAN_msg0x106_buf7_Raw;
extern unsigned int Susp_CAN_msg0x106_AIMtx_buf0_Raw, Susp_CAN_msg0x106_AIMtx_buf1_Raw, Susp_CAN_msg0x106_AIMtx_buf2_Raw, Susp_CAN_msg0x106_AIMtx_buf3_Raw, Susp_CAN_msg0x106_AIMtx_buf4_Raw, Susp_CAN_msg0x106_AIMtx_buf5_Raw, Susp_CAN_msg0x106_AIMtx_buf6_Raw, Susp_CAN_msg0x106_AIMtx_buf7_Raw;
extern unsigned int Susp_CAN_msg0x1AB_buf0_Raw, Susp_CAN_msg0x1AB_buf1_Raw, Susp_CAN_msg0x1AB_buf2_Raw, Susp_CAN_msg0x1AB_buf3_Raw, Susp_CAN_msg0x1AB_buf4_Raw, Susp_CAN_msg0x1AB_buf5_Raw, Susp_CAN_msg0x1AB_buf6_Raw, Susp_CAN_msg0x1AB_buf7_Raw;
extern unsigned int Susp_CAN_msg0x391_buf0_Raw, Susp_CAN_msg0x391_buf1_Raw, Susp_CAN_msg0x391_buf2_Raw, Susp_CAN_msg0x391_buf3_Raw, Susp_CAN_msg0x391_buf4_Raw, Susp_CAN_msg0x391_buf5_Raw, Susp_CAN_msg0x391_buf6_Raw, Susp_CAN_msg0x391_buf7_Raw;

extern unsigned int Susp_CAN_msg0x789_buf0_Raw, Susp_CAN_msg0x789_buf1_Raw, Susp_CAN_msg0x789_buf2_Raw, Susp_CAN_msg0x789_buf3_Raw, Susp_CAN_msg0x789_buf4_Raw, Susp_CAN_msg0x789_buf5_Raw, Susp_CAN_msg0x789_buf6_Raw, Susp_CAN_msg0x789_buf7_Raw;


extern  CAN_message_t   Susp_CAN_msg0x786;
extern  CAN_message_t   Susp_CAN_msg0x787;
extern  CAN_message_t   Susp_CAN_msg0x787_mux1;
extern  CAN_message_t   Susp_CAN_msg0x787_mux2;
extern  CAN_message_t   Susp_CAN_msg0x788_mux1;
extern  CAN_message_t   Susp_CAN_msg0x788_mux2;
extern  CAN_message_t   Susp_CAN_msg0x789;
extern  CAN_message_t   Susp_CAN_msg0x790;
extern  CAN_message_t   Susp_CAN_msg0x791;
extern  CAN_message_t   Susp_CAN_msg0x794_mux1;
extern  CAN_message_t   Susp_CAN_msg0x794_mux2;
extern  CAN_message_t   Susp_CAN_msg0x794_mux3;
extern  CAN_message_t   Susp_CAN_msg0x794_mux4;
extern  CAN_message_t   Susp_CAN_msg0x794_mux5;


extern CAN_message_t   Susp_CAN_msg0x786_forAIM;
extern CAN_message_t   Susp_CAN_msg0x787_forAIM;
extern CAN_message_t   Susp_CAN_msg0x789_forAIM;
extern CAN_message_t   Susp_CAN_msg0x792_forAIM;
extern CAN_message_t   Susp_CAN_msg0x793_forAIM;

extern CAN_message_t Susp_CAN_msg0x1AB; // Brake Pedal Switch Status
extern CAN_message_t Susp_CAN_msg0xAD; // Gear Lever/Gear Position Status
extern CAN_message_t Susp_CAN_msg0x120; // Cruise Stalk Static Position


extern CAN_message_t Susp_CAN_msg0xB2; // Wheel Speed Sensors
extern CAN_message_t Susp_CAN_msg0x86; // SteeringWheel Angles
extern CAN_message_t Susp_CAN_msg0x106; // Brake Pressures
extern CAN_message_t Susp_CAN_msg0x391; // AccelPedal
extern CAN_message_t Susp_CAN_msg0x788; // AccelPedal

extern elapsedMillis Susp_CAN_canTxInterval_0x786_forAIM;
extern elapsedMillis Susp_CAN_canTxInterval_0x787_forAIM;
extern elapsedMillis Susp_CAN_canTxInterval_0x789_forAIM;
extern elapsedMillis Susp_CAN_canTxInterval_0x790_forAIM;


#pragma endregion

#pragma region // Define Susp_CAN MQB Variables generated by AIM Dash and some Native - Susp_CAN - Operational and Driver Inputs


 extern word BrakePressureBAR_0x106_AIMtx_CAN;
 extern word BrakePressureBAR_0x106_AIMtx_Extracted;
 extern unsigned int BrakePressureBAR_0x106_AIMtx_Raw;
 extern unsigned int BrakePressureBAR_0x106_AIMtx_Int;
 extern float BrakePressureBAR_0x106_AIMtx_Float;
  
 extern word BrakeFlag_0x106_AIMtx_CAN;
 extern unsigned int BrakeFlag_0x106_AIMtx_Raw;
 extern unsigned int BrakeFlag_0x106_AIMtx_Int;

 extern word AccelPedal_0x106_AIMtx_CAN;
 extern unsigned int AccelPedal_0x106_AIMtx_Raw;
 extern unsigned int AccelPedal_0x106_AIMtx_Int;
 extern float AccelPedal_0x106_AIMtx_Float;

 extern word AccelPedal_0x788_AIMtx_CAN;
 extern unsigned int AccelPedal_0x788_AIMtx_Raw;
 extern unsigned int AccelPedal_0x788_AIMtx_Int;
 extern float AccelPedal_0x788_AIMtx_Float;


  extern word WhlSpdFR_fromABS_Raw_CAN;
  extern word WhlSpdFL_fromABS_Raw_CAN;
  extern word WhlSpdRL_fromABS_Raw_CAN;
  extern word WhlSpdRR_fromABS_Raw_CAN;
  extern word WhlSpdAvg_fromABS_Float_Raw_CAN;
 
  extern float WhlSpdFR_fromABS_Float_Raw;
  extern float WhlSpdFL_fromABS_Float_Raw;
  extern float WhlSpdRL_fromABS_Float_Raw;
  extern float WhlSpdRR_fromABS_Float_Raw;
  extern float WhlSpdAvg_fromABS_Float_Raw;
  extern float TrueWhlSpdRR_fromABS_Float_Raw;

  extern unsigned int WhlSpdFR_fromABS_Int;
  extern unsigned int WhlSpdFL_fromABS_Int;
  extern unsigned int WhlSpdRL_fromABS_Int;
  extern unsigned int WhlSpdRR_fromABS_Int;
  extern unsigned int WhlSpdAvg_fromABS_Int;


  extern unsigned int TFT_MITM_LC_BumpIn_TrueWhlSpdRR_fromABS_Int;
  extern float        TFT_MITM_LC_BumpIn_TrueWhlSpdRR_fromABS_Float;
  extern unsigned int TFT_MITM_LC_BumpIn_TrueWhlSpdRR_fromABS_8bit;
  extern unsigned int TFT_MITM_LC_BumpIn_TrueWhlSpdRR_fromABS_canTx_16bit;


  extern word SuspCAN_WhlSpdFRfromAIM_CAN;
  extern word SuspCAN_WhlSpdFLfromAIM_CAN;
  extern word SuspCAN_WhlSpdRLfromAIM_CAN;
  extern word SuspCAN_WhlSpdRRfromAIM_CAN;


  extern unsigned int SuspCAN_VehSpeedfromAIM_Int;
  extern unsigned int SuspCAN_WhlSpdFRfromAIM_Int;
  extern unsigned int SuspCAN_WhlSpdFLfromAIM_Int;
  extern unsigned int SuspCAN_WhlSpdRLfromAIM_Int;
  extern unsigned int SuspCAN_WhlSpdRRfromAIM_Int;
  extern unsigned int SuspCAN_WhlSpdAvgfromAIM_Int;

  extern unsigned int SuspCAN_WhlSpdFRfromAIM_Raw;
  extern unsigned int SuspCAN_WhlSpdFLfromAIM_Raw;
  extern unsigned int SuspCAN_WhlSpdRLfromAIM_Raw;
  extern unsigned int SuspCAN_WhlSpdRRfromAIM_Raw;
  extern unsigned int SuspCAN_WhlSpdAvgfromAIM_Raw_Int;

  extern float SuspCAN_VehSpeedfromAIM_Float;
  extern float SuspCAN_WhlSpdFRfromAIM_Float;
  extern float SuspCAN_WhlSpdFLfromAIM_Float;
  extern float SuspCAN_WhlSpdRLfromAIM_Float;
  extern float SuspCAN_WhlSpdRRfromAIM_Float;
  extern float SuspCAN_WhlSpdAvgfromAIM_Float;

  extern float SuspCAN_WhlSpdFRfromAIM_Float_Raw;
  extern float SuspCAN_WhlSpdFLfromAIM_Float_Raw;
  extern float SuspCAN_WhlSpdRLfromAIM_Float_Raw;
  extern float SuspCAN_WhlSpdRRfromAIM_Float_Raw;
  extern float SuspCAN_WhlSpdAvgfromAIM_Float_Raw;

  extern unsigned char SuspCAN_WhlSpdFRfromAIM_Char;
  extern unsigned char SuspCAN_WhlSpdFLfromAIM_Char;
  extern unsigned char SuspCAN_WhlSpdRLfromAIM_Char;
  extern unsigned char SuspCAN_WhlSpdRRfromAIM_Char;
  extern unsigned char SuspCAN_WhlSpdAvgfromAIM_Char;


  extern float SuspCAN_WhlSpdfromAIM_FrontAxleAvg_Float;
  extern float SuspCAN_WhlSpdfromAIM_RearAxleAvg_Float;
  extern float SuspCAN_WhlSpdfromAIM_All_Avg_Float;
 
  extern float SuspCAN_WhlSpdfromAIM_FrontAxleSlipKmh_Float;
  extern float SuspCAN_WhlSpdfromAIM_RearAxleSlipKmh_Float;
  extern float SuspCAN_WhlSpdfromAIM_CenterSlipKmh_Float;

  extern float SuspCAN_WhlSpdfromAIM_FrontAxleSlipPct_Float;
  extern float SuspCAN_WhlSpdfromAIM_RearAxleSlipPct_Float;
  extern float SuspCAN_WhlSpdfromAIM_CenterSlipPct_Float;

extern float WhlSpd_FrontAxleAvg_Float;
extern float WhlSpd_RearAxleAvg_Float;
extern float WhlSpd_All_Avg_Float;
extern float WhlSpdAvg_Float;


extern float WhlSpd_FrontAxleSlipKmh_Float;
extern float WhlSpd_RearAxleSlipKmh_Float;
extern float WhlSpd_CenterSlipKmh_Float;
 
extern float WhlSpd_FrontAxleSlipPct_Float;
extern float WhlSpd_RearAxleSlipPct_Float;
extern float WhlSpd_CenterSlipPct_Float;


  extern word ThrottleTPS_SuspCAN_0x391_CAN;
  extern word AccelPedal_SuspCAN_0x391_CAN;
  extern unsigned int AccelPedal_SuspCAN_0x391;
  extern unsigned int AccelPedal_SuspCAN_0x391_Mapped;
  extern float AccelPedal_SuspCAN_0x391_Mapped_Float;
 
  extern unsigned int ThrottleTPS_SuspCAN_0x391;



  extern word MQB_SteeringWheelAngle_SuspCAN_0x86_CAN;
  extern word MQB_SteeringWheelAngle_SuspCAN_0x86_Extracted;
  extern unsigned int MQB_SteeringWheelAngle_SuspCAN_0x86_Raw;
  extern float MQB_SteeringWheelAngle_SuspCAN_0x86_Float;
  extern unsigned int MQB_SteeringWheelAngle_SuspCAN_0x86_Int;
  extern float MQB_SteeringWheelAngle_SuspCAN_0x86_Radians_Int;





#pragma endregion

#pragma region // Operational Variables from from PT_CAN Sniffing



// Operational   

extern word IgnitionStatusTimerCAN;

extern unsigned int mapswitch;
extern unsigned int DS1CurrentMapSlot_0x7EE; 

extern unsigned int PowerGauge_Converted;
extern unsigned int TorqueGauge_Converted;


extern signed int rpmDOT_5ms;
extern signed int rpmDOT_10ms;
extern signed int rpmDOT_20ms;
extern signed int rpmDOT_20ms_KalmanFiltered;
extern signed int rpmDOT_33ms;
extern signed int rpmDOT_50ms;
extern signed int rpmDOT_100ms;

// extern unsigned int lastRPM_5ms;
// extern unsigned int lastRPM_10ms;
// extern unsigned int lastRPM_20ms;
// extern unsigned int lastRPM_33ms;
// extern unsigned int lastRPM_50ms;
// extern unsigned int lastRPM_100ms;

extern unsigned int EngineRPM_last_5ms;
extern unsigned int EngineRPM_last_10ms;
extern unsigned int EngineRPM_last_20ms;
extern unsigned int EngineRPM_last_33ms;
extern unsigned int EngineRPM_last_50ms;
extern unsigned int EngineRPM_last_100ms;

extern unsigned int KalmanFiltered_EngineRPM_last_5ms;
extern unsigned int KalmanFiltered_EngineRPM_last_10ms;
extern unsigned int KalmanFiltered_EngineRPM_last_20ms;
extern unsigned int KalmanFiltered_EngineRPM_last_33ms;
extern unsigned int KalmanFiltered_EngineRPM_last_50ms;
extern unsigned int KalmanFiltered_EngineRPM_last_100ms;


extern unsigned int cruiseStalk;
extern unsigned int EngineRPM_IdleStatus;
extern unsigned int EngineRPM_0xA8;
extern unsigned int EngineRPM_0x783;
extern unsigned int EngineRPM_0x788;
extern unsigned int EngineRPM_0xA8;

extern  float EngineRPM_0x783_Float;
extern  float EngineRPM_0x788_Float;
extern  float EngineRPM_0xA8_Float;

extern unsigned int EngineRPM_DOT_0x783;
extern unsigned int EngineRPM_DOT_0x788;

extern unsigned int temp_EngRPM;
extern unsigned int EngineRPM_ON;
extern unsigned int EngineRPM_OFF;

extern unsigned int MQB_TachEngineRPM_0x107;
extern unsigned int MQB_TachEngineRPM_Sniffed_0x107_Raw;
extern unsigned int MQB_TachEngineRPM_Sniffed_0x107_Int;
extern unsigned int MQB_TachEngineRPM_Override_0x107_Raw;
extern unsigned int MQB_TachEngineRPM_Override_0x107_Int;
extern unsigned int DisplayTachometerOverride_View_Full;
extern unsigned int MQB_TachEngineRPM_Override_View_8bit;

extern int VehSpeed;
extern int WhlSpdFR;
extern int WhlSpdFL;
extern int WhlSpdRL;
extern int WhlSpdRR;
extern int WhlSpdAvg_Int;

extern int WhlSpdFR_Raw;
extern int WhlSpdFL_Raw;
extern int WhlSpdRL_Raw;
extern int WhlSpdRR_Raw;
extern int WhlSpdAvg_Raw_Int; 

extern float VehSpeed_Float;
extern float WhlSpdFR_Float;
extern float WhlSpdFL_Float;
extern float WhlSpdRL_Float;
extern float WhlSpdRR_Float;
extern float WhlSpdAvg_Float;

extern unsigned char WhlSpdFR_Char;
extern unsigned char WhlSpdFL_Char;
extern unsigned char WhlSpdRL_Char;
extern unsigned char WhlSpdRR_Char;
extern unsigned char WhlSpdAvg_Char;


extern float WhlSpdFR_Float_Raw;
extern float WhlSpdFL_Float_Raw;
extern float WhlSpdRL_Float_Raw;
extern float WhlSpdRR_Float_Raw;
extern float WhlSpdAvg_Float_Raw;


extern float WhlSpd_FrontAxleAvg_Float;
extern float WhlSpd_RearAxleAvg_Float;
extern float WhlSpd_All_Avg_Float;
extern float WhlSpdAvg_Float;


extern float WhlSpd_FrontAxleSlipKmh_Float;
extern float WhlSpd_RearAxleSlipKmh_Float;
extern float WhlSpd_CenterSlipKmh_Float;
 
extern float WhlSpd_FrontAxleSlipPct_Float;
extern float WhlSpd_RearAxleSlipPct_Float;
extern float WhlSpd_CenterSlipPct_Float;





extern unsigned int DQ500_TargetGear_0x394;
extern unsigned int DQ500_TargetGear2_0x394;
extern unsigned int DQ500_CurrentGear_0x394;
extern unsigned int DQ500_CurrentGear_0x7EE;

#pragma endregion

#pragma region // Declarations of MQB Executable CAN Function Allowables (DQ500 Resets, AutoUPshift Modes, etc)
extern bool PT_CAN_DQ500_DAQ_OLD_Set1_SequenceComplete;
extern bool OBD_CAN_DQ500_DAQ_OLD_Set1_SequenceComplete;
extern bool DQ500_DAQ_OLD_Set1_SequenceComplete;
extern bool DQ500Reset_ClutchCharacteristicCurve_HasRun;
extern unsigned int DQ500Reset_ClutchCharacteristicCurve_Allowed;
extern unsigned int DQ500Reset_ClutchCharacteristicCurve_Trigger;

extern bool DQ500Reset_DblClutchFastAdaptation_HasRun;
extern unsigned int DQ500Reset_DblClutchFastAdaptation_Allowed;
extern unsigned int DQ500Reset_DblClutchFastAdaptation_Trigger;

extern bool DQ500Reset_ResetBasicSettings_HasRun;
extern unsigned int DQ500Reset_ResetBasicSettings_Allowed;
extern unsigned int DQ500Reset_ResetBasicSettings_Trigger;

extern unsigned int DQ500Reset_BrakePedalAcceptableCondition;
extern unsigned int DQ500Reset_EngRPMAcceptableCondition;
extern unsigned int DQ500Reset_GearLeverPositionAcceptableCondition;
extern unsigned int DQ500Reset_CruiseStalkAcceptableCondition;

extern unsigned int DQ500_Reset_Mode;
extern unsigned int DQ500_Reset_ClutchCharacteristic_Mode;
extern unsigned int DQ500_Reset_DblClutchAdapations_Mode;
extern unsigned int DQ500_Reset_BasicSettings_Mode;

extern unsigned int DQ500Reset_ClutchCharacteristic_AcceptableConditions;
extern unsigned int DQ500Reset_DblClutchAdaptations_AcceptableConditions;
extern unsigned int DQ500Reset_BasicSettings_AcceptableConditions;


//  Driver Configuration or Activation Modes 

extern unsigned int CarConfig_DisplayGaugesOverrideMode_Int;
extern unsigned int CarConfig_DisplayGaugesOverride_UpdateMode_Active;
extern unsigned int CarConfig_DisplayGaugesOverride_Int_current;

extern unsigned int DisplayGaugesOverride_Active;
extern unsigned int DisplayTachometerOverride_Converted_FinalInt;
extern unsigned char lsbMQB_DisplayTachometerOverride_Converted_FinalInt, msbMQB_DisplayTachometerOverride_Converted_FinalInt;



extern unsigned int DynoMode_JobMode;
extern unsigned int DynoModeStatus;

extern unsigned int PT_CAN_LPFP_Activation_Mode;
extern unsigned int PT_CAN_LPFP_Activation_Mode_Active;
extern unsigned int PT_CAN_LPFP_Activation_Mode_ContinousActive;

extern unsigned int OBD_DTC_Clear_Mode;
extern unsigned int OBD_DTC_Clear;
extern unsigned int PT_CAN_DTC_Clear_Mode;
extern unsigned int PT_CAN_DTC_Clear;

extern unsigned int Susp_LvlSnsr_Offsets_Reset_Mode_Active;

extern unsigned int CarConfig_SuspLevelSensors_UpdateMode_Active;
extern unsigned int CarConfig_SuspLevelSensors_Int;
extern unsigned int CarConfig_SuspLevelSensors_Int_current;

extern unsigned int CarConfig_AWDController_UpdateMode_Active;
extern unsigned int CarConfig_AWDController_Int;
extern unsigned int CarConfig_AWDController_Int_current;


extern unsigned int Temp7TypeConfig_Mode_Active;
extern unsigned int Temp7_Type;
extern unsigned int Temp8TypeConfig_Mode_Active;
extern unsigned int Temp8_Type;

extern unsigned int SerialOutput_Configuration_Mode;
extern unsigned int SerialOutput_Configuration;
extern unsigned int shouldbedisplayingSerialOutputMode;



extern unsigned int PT_CAN_DTC_Clear_Mode_Active;
// extern unsigned int MQB_SRI_Reset_AcceptableConditions;
extern unsigned int DTC_Clear_Msg_Sequence;
extern unsigned int DynoMode_ActivationMsg_Sequence;

// ------------------------------------------------------------------------


extern unsigned int DynoModeStatus_TCU;
extern unsigned int DynoModeStatus_ECU;
extern unsigned int DynoModeStatus_ABS;
extern unsigned int DynoModeStatus_FrontSensorsDriverAssist;
extern unsigned int DynoModeStatus_Dashboard;
extern unsigned int DynoModeStatus_ACC;
extern unsigned int DynoModeStatus_Gateway;
extern unsigned int DynoModeStatus_ICU;
extern unsigned int DynoModeStatus_LaneChangeAssist;
extern unsigned int DynoModeStatus_AWD;


extern unsigned int DynoMode_Read_Message_Sequence;
extern unsigned int DynoMode_Activation_Message_Sequence;
extern unsigned int DynoMode_DEActivation_Message_Sequence;
 
extern unsigned int DynoMode_Read_Phase;
extern unsigned int DynoMode_Activation_Phase;
extern unsigned int DynoMode_DEActivation_Phase;
 
 
extern unsigned int DynoMode_Read_InProcess;
extern unsigned int DynoMode_Activation_InProcess;
extern unsigned int DynoMode_DEActivation_InProcess;


extern unsigned int ModuleOnline_TCU;
extern unsigned int ModuleOnline_ECU;
extern unsigned int ModuleOnline_ABS;
extern unsigned int ModuleOnline_FrontSensorsDriverAssist;
extern unsigned int ModuleOnline_Dashboard;
extern unsigned int ModuleOnline_ACC;
extern unsigned int ModuleOnline_Gateway;
extern unsigned int ModuleOnline_ICU;
extern unsigned int ModuleOnline_LaneChangeAssist;
extern unsigned int ModuleOnline_AWD;
 
extern unsigned int CombinedDynoModeStatus_ModulesActive;
extern unsigned int CombinedDynoModeStatus_ModulesOnline;

extern unsigned int FuelTankLevel;

// ------------------------------------------------------------------------

extern unsigned int DriveSelect_EngTrans_Mode_Raw;
extern unsigned int DriveSelect_Suspension_Mode_Raw;
extern unsigned int DriveSelect_Quattro_Mode_Raw;
extern unsigned int DriveSelect_Steering_Mode_Raw;
extern unsigned int DriveSelect_EngineSound_Mode_Raw;
extern unsigned int DriveSelect_ACC_Mode_Raw;

extern unsigned int DriveSelect_EngTrans_Mode_Int;
extern unsigned int DriveSelect_Suspension_Mode_Int;
extern unsigned int DriveSelect_Quattro_Mode_Int;
extern unsigned int DriveSelect_Steering_Mode_Int;
extern unsigned int DriveSelect_EngineSound_Mode_Int;
extern unsigned int DriveSelect_ACC_Mode_Int;


extern unsigned int DriveSelect_EngTrans_Mode_Comfort;
extern unsigned int DriveSelect_EngTrans_Mode_Auto;
extern unsigned int DriveSelect_EngTrans_Mode_Dynamic;

extern unsigned int DriveSelect_Suspension_Mode_Comfort;
extern unsigned int DriveSelect_Suspension_Mode_Auto;
extern unsigned int DriveSelect_Suspension_Mode_Dynamic;

extern unsigned int DriveSelect_Quattro_Mode_Comfort;
extern unsigned int DriveSelect_Quattro_Mode_Auto;
extern unsigned int DriveSelect_Quattro_Mode_Dynamic;

extern unsigned int DriveSelect_Steering_Mode_Comfort;
extern unsigned int DriveSelect_Steering_Mode_Auto;
extern unsigned int DriveSelect_Steering_Mode_Dynamic;

extern unsigned int DriveSelect_EngineSound_Mode_Comfort;
extern unsigned int DriveSelect_EngineSound_Mode_Auto;
extern unsigned int DriveSelect_EngineSound_Mode_Dynamic;

extern unsigned int DriveSelect_ACC_Mode_Comfort;
extern unsigned int DriveSelect_ACC_Mode_Auto;
extern unsigned int DriveSelect_ACC_Mode_Dynamic;

// KalmanFilter Variables  ------------------------------------------------------------------------

extern unsigned int MQB_AutoShift_UpdateMode_EngineRPM_KalmanFilteredMPC_Gain_Active;
extern unsigned int MQB_AutoShift_EngineRPM_KalmanFilteredMPC_Gain_Int;
extern float MQB_AutoShift_EngineRPM_KalmanFilteredMPC_Gain_Float;

extern unsigned int TFT_KalmanFilteredMPC_PredictedEngRPM_Extrapolated200ms;
extern unsigned int TFT_KalmanFilteredMPC_PredictedEngRPM_Extrapolated200ms_Filtered;


extern float MQB_AutoShift_EngineRPM_KalmanFilter_Gain_Float;
extern float DeltaValue;
extern float measuredRPM_0x783;
extern float measuredRPM_0x788;
extern float measuredRPM_0x0A8;
extern float real_value;
extern float measured_value;
extern float estimated_value;
extern float TFT_KalmanFilteredMPC_FilteredEngRPM;
extern unsigned int TFT_KalmanFilteredMPC_FilteredEngRPM_Int;

// AutoShift Setpoints and Trigger Function variables  ------------------------------------------------------------------------


extern unsigned int MQB_AutoShift_via_PaddleSignalGeneration_Allowed;
extern unsigned int MQB_AutoShift_via_PaddleSignalGeneration_AcceptableCondition;
extern unsigned int PaddleUP_SignalGeneration_Active;
extern unsigned int PaddleDOWN_SignalGeneration_Active;

extern unsigned int MQB_AutoUPshift_Gear1to2_RPMSetpoint_Int;
extern unsigned int MQB_AutoUPshift_Gear2to3_RPMSetpoint_Int;
extern unsigned int MQB_AutoUPshift_Gear3to4_RPMSetpoint_Int;
extern unsigned int MQB_AutoUPshift_Gear4to5_RPMSetpoint_Int;
extern unsigned int MQB_AutoUPshift_Gear5to6_RPMSetpoint_Int;
extern unsigned int MQB_AutoUPshift_Gear6to7_RPMSetpoint_Int;

extern unsigned int MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Int;


extern float MQB_AutoUPshift_Gear1to2_RPMSetpoint_Trigger_Float;
extern float MQB_AutoUPshift_Gear2to3_RPMSetpoint_Trigger_Float;
extern float MQB_AutoUPshift_Gear3to4_RPMSetpoint_Trigger_Float;
extern float MQB_AutoUPshift_Gear4to5_RPMSetpoint_Trigger_Float;
extern float MQB_AutoUPshift_Gear5to6_RPMSetpoint_Trigger_Float;
extern float MQB_AutoUPshift_Gear6to7_RPMSetpoint_Trigger_Float;

extern float MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Float;


extern unsigned int MQB_AutoUPshift_Gear1to2_RPMSetpoint_Trigger_Int;
extern unsigned int MQB_AutoUPshift_Gear2to3_RPMSetpoint_Trigger_Int;
extern unsigned int MQB_AutoUPshift_Gear3to4_RPMSetpoint_Trigger_Int;
extern unsigned int MQB_AutoUPshift_Gear4to5_RPMSetpoint_Trigger_Int;
extern unsigned int MQB_AutoUPshift_Gear5to6_RPMSetpoint_Trigger_Int;
extern unsigned int MQB_AutoUPshift_Gear6to7_RPMSetpoint_Trigger_Int;


extern unsigned int MQB_AutoDOWNshift_Gear2to1_RPMSetpoint_Int;
extern unsigned int MQB_AutoDOWNshift_Gear3to2_RPMSetpoint_Int;
extern unsigned int MQB_AutoDOWNshift_Gear4to3_RPMSetpoint_Int;
extern unsigned int MQB_AutoDOWNshift_Gear5to4_RPMSetpoint_Int;
extern unsigned int MQB_AutoDOWNshift_Gear6to5_RPMSetpoint_Int;
extern unsigned int MQB_AutoDOWNshift_Gear7to6_RPMSetpoint_Int;


extern unsigned int MQB_AutoShift_RPMSetpoint_UpdateSetpoints_AcceptableConditions;
extern unsigned int MQB_AutoShift_UpdateMode_GearShifttoUpdate;
extern unsigned int MQB_AutoShift_UpdateMode_GearShifttoUpdateConvertedForDisplay;
extern unsigned int MQB_AutoShift_UpdateMode_RPMSetpointConvertedForDisplay;
extern unsigned int MQB_AutoShift_UpdateMode_RPMSetpoint_Int_Full;

extern unsigned int MQB_AutoShift_UpdateMode_EEPROM_Update_ArmedStatus;
extern unsigned int MQB_AutoShift_UpdateMode_OBD_Service22_RequestsEnable;



extern unsigned int MQB_AutoUPshift_RPMSetpoint_Gear1to2_RPMUpdateModeActive;
extern unsigned int MQB_AutoUPshift_RPMSetpoint_Gear2to3_RPMUpdateModeActive;
extern unsigned int MQB_AutoUPshift_RPMSetpoint_Gear3to4_RPMUpdateModeActive;
extern unsigned int MQB_AutoUPshift_RPMSetpoint_Gear4to5_RPMUpdateModeActive;
extern unsigned int MQB_AutoUPshift_RPMSetpoint_Gear5to6_RPMUpdateModeActive;
extern unsigned int MQB_AutoUPshift_RPMSetpoint_Gear6to7_RPMUpdateModeActive;

extern unsigned int MQB_AutoDOWNshift_RPMSetpoint_Gear2to1_RPMUpdateModeActive;
extern unsigned int MQB_AutoDOWNshift_RPMSetpoint_Gear3to2_RPMUpdateModeActive;
extern unsigned int MQB_AutoDOWNshift_RPMSetpoint_Gear4to3_RPMUpdateModeActive;
extern unsigned int MQB_AutoDOWNshift_RPMSetpoint_Gear5to4_RPMUpdateModeActive;
extern unsigned int MQB_AutoDOWNshift_RPMSetpoint_Gear6to5_RPMUpdateModeActive;
extern unsigned int MQB_AutoDOWNshift_RPMSetpoint_Gear7to6_RPMUpdateModeActive;


extern unsigned int MQB_AutoShift_RPMSetpoints_ALL_EEPROM_UpdateActive;
extern unsigned int MQB_AutoUPshift_RPMSetpoint_Gear1to2_EEPROM_UpdateActive;
extern unsigned int MQB_AutoUPshift_RPMSetpoint_Gear2to3_EEPROM_UpdateActive;
extern unsigned int MQB_AutoUPshift_RPMSetpoint_Gear3to4_EEPROM_UpdateActive;
extern unsigned int MQB_AutoUPshift_RPMSetpoint_Gear4to5_EEPROM_UpdateActive;
extern unsigned int MQB_AutoUPshift_RPMSetpoint_Gear5to6_EEPROM_UpdateActive;
extern unsigned int MQB_AutoUPshift_RPMSetpoint_Gear6to7_EEPROM_UpdateActive;

extern unsigned int MQB_AutoDOWNshift_RPMSetpoint_Gear2to1_EEPROM_UpdateActive;
extern unsigned int MQB_AutoDOWNshift_RPMSetpoint_Gear3to2_EEPROM_UpdateActive;
extern unsigned int MQB_AutoDOWNshift_RPMSetpoint_Gear4to3_EEPROM_UpdateActive;
extern unsigned int MQB_AutoDOWNshift_RPMSetpoint_Gear5to4_EEPROM_UpdateActive;
extern unsigned int MQB_AutoDOWNshift_RPMSetpoint_Gear6to5_EEPROM_UpdateActive;
extern unsigned int MQB_AutoDOWNshift_RPMSetpoint_Gear7to6_EEPROM_UpdateActive;




// LaunchControl EngineRPM PID Controller  ------------------------------------------------------------------------

extern double LC_EngineRPM_PIDoutput_TotalIgnCorrection_sFloat; // PID Controlled Ign Output for EngineRPM for LC and TC
extern signed int LC_EngineRPM_PIDoutput_TotalIgnCorrection_sInt; // PID Controlled Ign Output for EngineRPM for LC and TC
extern unsigned int LC_EngineRPM_PIDoutput_TotalIgnCorrection_Int; // PID Controlled Ign Output for EngineRPM for LC and TC
extern double LC_EngRPM_PID_OutputIgn_Double; // Direct output of PID controller

extern double EngRPM_Setpoint_Double;
extern unsigned int EngRPM_Setpoint_Int;
extern double measuredRPM;

extern unsigned int measuredRPM_Int;

extern double kp;   // * (P)roportional Tuning Parameter *FROM PID_v1 Library
extern double ki;   // * (I)ntegral Tuning Parameter *FROM PID_v1 Library
extern double kd;   // * (D)erivative Tuning Parameter *FROM PID_v1 Library

extern double input;
extern double error;
extern double dInput;




// LaunchControl RPM Setpoints and BoostWindow  ------------------------------------------------------------------------

extern unsigned int MQB_LaunchControl_RampModeActiveStatus;
extern unsigned int MQB_LaunchControl_HoldModeActiveStatus;
extern unsigned int MQB_LaunchControl_TransitionModeActiveStatus;


extern unsigned int MQB_TFT_EngineRPM_Setpoint_Int;
extern unsigned int MQB_TFT_EngineRPM_Setpoint_Int_8bit;
extern unsigned int MQB_LaunchControl_EngineRPM_Ramp_Setpoint_Int;
extern unsigned int MQB_LaunchControl_EngineRPM_Ramp_Setpoint_Int_8bit;
extern unsigned int MQB_LaunchControl_EngineRPM_NewHold_Setpoint_Int;
extern unsigned int MQB_LaunchControl_EngineRPM_Hold_Setpoint_Int;
extern unsigned int MQB_LaunchControl_EngineRPM_Hold_Setpoint_Int_8bit;
extern unsigned int MQB_LaunchControl_EngineRPM_Transition_Setpoint_Int;
extern unsigned int MQB_LaunchControl_EngineRPM_Transition_Setpoint_Int_8bit;

extern float MQB_LaunchControl_EngineRPM_TransitionRateMultiplier_Float;
extern unsigned int MQB_LaunchControl_EngineRPM_TransitionRateMultiplier_Int;


extern unsigned int MQB_LaunchControl_EngineRPM_PID_BaseOffset_Int;
extern signed int LC_IgnOffsetBaseSigned;

extern float MQB_LaunchControl_EngineRPM_PID_Ki_Term_Float;
extern unsigned int MQB_LaunchControl_EngineRPM_PID_Ki_Term_Int;
extern unsigned int MQB_LaunchControl_EngineRPM_RAMP_Timeout_Int;

extern signed int MQB_LaunchControl_EngineRPM_Setpoint_CurrentDelta;
extern signed int MQB_LaunchControl_EngineRPM_Setpoint_CurrentDelta_16bitSigned;
extern signed int MQB_LaunchControl_EngineRPM_Setpoint_CurrentDelta_8bit;

extern signed int MQB_IgnTimingCorrection;
extern signed int MQB_IgnTimingCorrection_8bitSigned;



extern unsigned int MQB_LaunchControl_BoostWindow_EntryThresholdPressure_Int;
extern unsigned int MQB_LaunchControl_BoostWindow_minMaintainPressure_Int;
extern unsigned int MQB_LaunchControl_BoostVariableSelection_Int;


extern unsigned int MQB_LaunchControl_Variable_Update_AcceptableCondition;
extern unsigned int MQB_LaunchControl_EngineRPM_Setpoint_Update_AcceptableCondition;
extern unsigned int MQB_LaunchControl_BoostWindow_EntryThresholdPressure_Update_AcceptableCondition;
extern unsigned int MQB_LaunchControl_BoostWindow_minMaintainPressure_Update_AcceptableCondition;
extern unsigned int MQB_LaunchControl_UpdateMode_BoostVariableSelection_Active;


extern unsigned int MQB_LaunchControl_UpdateMode_EngineRPM_Ramp_Setpoint_Active;
extern unsigned int MQB_LaunchControl_UpdateMode_EngineRPM_Hold_Setpoint_Active;
extern unsigned int MQB_LaunchControl_UpdateMode_EngineRPM_TransitionRateMultiplier_Active;
extern unsigned int MQB_LaunchControl_UpdateMode_EngineRPM_RAMP_Timeout_Active;
extern unsigned int MQB_LaunchControl_UpdateMode_EngineRPM_PID_BaseOffset_Active;
extern unsigned int MQB_LaunchControl_UpdateMode_EngineRPM_PID_Ki_Term_Active;


extern unsigned int MQB_LaunchControl_BoostWindow_EntryThresholdPressure_Achieved;
extern unsigned int MQB_LaunchControl_BoostWindow_EntryThresholdPressure_Hysteresis;
extern unsigned int MQB_LaunchControl_BoostWindow_Maintained_Status;
extern unsigned int MQB_LaunchControl_CombinedSignalActive;
extern unsigned int MQB_LaunchControl_CombinedSignalActive_8bit;


extern unsigned int MQB_LaunchControl_UpdateMode_VariabletoUpdate;
extern unsigned int MQB_LaunchControl_UpdateMode_VariabletoUpdateConvertedForDisplay;
extern unsigned int MQB_LaunchControl_UpdateMode_RPMSetpointConvertedForDisplay;
extern unsigned int MQB_LaunchControl_UpdateMode_BoostWindow_EntryThresholdPressure_ConvertedForDisplay;
extern unsigned int MQB_LaunchControl_UpdateMode_BoostWindow_minMaintainPressure_ConvertedForDisplay;
extern unsigned int MQB_LaunchControl_UpdateMode_EEPROM_Update_ArmedStatus;
extern unsigned int MQB_LaunchControl_UpdateMode_OBD_Service22_RequestsEnable;


extern unsigned int MQB_LaunchControl_UpdateMode_BoostVariableSelection_Active;
extern unsigned int MQB_LaunchControl_UpdateMode_BoostWindow_EntryThresholdPressure_Active;
extern unsigned int MQB_LaunchControl_UpdateMode_BoostWindow_minMaintainPressure_Active;

extern unsigned int MQB_LaunchControl_AllSetpoints_EEPROM_UpdateActive;
extern unsigned int MQB_LaunchControl_BoostWindow_EntryThresholdPressure_EEPROM_UpdateActive;
extern unsigned int MQB_LaunchControl_BoostWindow_minMaintainPressure_EEPROM_UpdateActive;



// TractionControl Setpoints   ------------------------------------------------------------------------

extern unsigned int MQB_TractionControl_TgtSlipRate_Int;
extern unsigned int MQB_TractionControl_CorrectionAggression_Int;

extern unsigned int MQB_TractionControl_Variable_Update_AcceptableCondition;

extern unsigned int MQB_TractionControl_UpdateMode_VariabletoUpdate;
extern unsigned int MQB_TractionControl_UpdateMode_VariabletoUpdateConvertedForDisplay;

extern unsigned int MQB_TractionControl_UpdateMode_TgtSlipRate_ConvertedForDisplay;
extern unsigned int MQB_TractionControl_UpdateMode_CorrectionAggression_ConvertedForDisplay;

extern unsigned int MQB_TractionControl_UpdateMode_EEPROM_Update_ArmedStatus;
extern unsigned int MQB_TractionControl_UpdateMode_OBD_Service22_RequestsEnable;

extern unsigned int MQB_TractionControl_UpdateMode_TgtSlipRate_Active;
extern unsigned int MQB_TractionControl_UpdateMode_CorrectionAggression_Active;

extern unsigned int MQB_TractionControl_Settings_EEPROM_UpdateActive;

// BurnoutMode Setpoints   ------------------------------------------------------------------------

extern unsigned int MQB_BurnoutMode_EngineRPM_Setpoint_Int;
extern unsigned int MQB_BurnoutMode_EngineRPM_Setpoint_Int_8bit;
extern unsigned int MQB_BurnoutMode_UpdateMode_EngineRPM_Setpoint_Active;
extern unsigned int MQB_BurnoutMode_ActiveStatus;


// Parachute Setpoints   ------------------------------------------------------------------------

extern unsigned int MQB_Parachute_PreArmedStatus;
extern unsigned int MQB_Parachute_UpdateMode_PreArmedStatus_Active;
extern unsigned int MQB_Parachute_PreArmedStatus_ActiveStatus;

extern unsigned int MQB_Parachute_ArmSpeed_Setpoint_Int;
extern unsigned int MQB_Parachute_ArmSpeed_Setpoint_Int_8bit;

// extern unsigned int MQB_Parachute_ArmSpeed_Setpoint_Allowed = 1;
// extern unsigned int MQB_Parachute_ArmSpeed_Setpoint_AcceptableCondition = 0;
extern unsigned int MQB_Parachute_ArmSpeed_Setpoint_ActiveStatus;

extern unsigned int MQB_Parachute_UpdateMode_ArmSpeed_Setpoint;
extern unsigned int MQB_Parachute_UpdateMode_ArmSpeed_Setpoint_Active;
extern unsigned int MQB_Parachute_ArmSpeed_Setpoint_EEPROM_UpdateActive;


#pragma endregion

#pragma region // Declarations of MQB + TFT Configuration Variables
// Fuel Overrun Override Mode Setpoints   ------------------------------------------------------------------------

extern unsigned int MQB_FuelOverrun_OverrideModeActive_Status;
extern unsigned int MQB_FuelOverrun_OverrideMode_Allowed;

// BoostByGear Multiplier Setpoints   ------------------------------------------------------------------------

extern unsigned int MQB_BoostByGearMultiplier_ActivationStatus_Int;
extern unsigned int MQB_BoostByGearMultiplier_Gear0_Int;
extern unsigned int MQB_BoostByGearMultiplier_Gear1_Int;
extern unsigned int MQB_BoostByGearMultiplier_Gear2_Int;
extern unsigned int MQB_BoostByGearMultiplier_Gear3_Int;
extern unsigned int MQB_BoostByGearMultiplier_Gear4_Int;
extern unsigned int MQB_BoostByGearMultiplier_Gear5_Int;
extern unsigned int MQB_BoostByGearMultiplier_Gear6_Int;
extern unsigned int MQB_BoostByGearMultiplier_Gear7_Int;

extern unsigned int MQB_BoostByGearMultiplier_ActivationStatus_UpdateMode_AcceptableConditions;
extern unsigned int MQB_BoostByGearMultiplier_UpdateMode_AcceptableConditions;

extern unsigned int MQB_BoostByGearMultiplier_ActivationStatus_UpdateActive;
extern unsigned int MQB_BoostByGearMultiplier_Gear0_UpdateModeActive;
extern unsigned int MQB_BoostByGearMultiplier_Gear1_UpdateModeActive;
extern unsigned int MQB_BoostByGearMultiplier_Gear2_UpdateModeActive;
extern unsigned int MQB_BoostByGearMultiplier_Gear3_UpdateModeActive;
extern unsigned int MQB_BoostByGearMultiplier_Gear4_UpdateModeActive;
extern unsigned int MQB_BoostByGearMultiplier_Gear5_UpdateModeActive;
extern unsigned int MQB_BoostByGearMultiplier_Gear6_UpdateModeActive;
extern unsigned int MQB_BoostByGearMultiplier_Gear7_UpdateModeActive;



// Display Gauge and Tachometer Overrides Arrays   ------------------------------------------------------------------------

  extern unsigned int PowerGaugeValue_DecSUM_Array[]; 
  //                                        = { 
  //                                        0, 416, 832, 1280, 1696, 2112, 2560, 2976, 3392, 3840, 4256, 4672, 5120, 5536, 5952, 6400, 6816, 7232, 7680, 8096, 8512, 8960, 9376, 9792, 
  //                                        10240, 10656, 11072, 11520, 11936, 12352, 12800, 13216, 13632, 14080, 14496, 14912, 15360, 15776, 16192, 16640, 17056, 17472, 17920, 18336, 18752, 19200, 19616, 
  //                                        20032, 20480, 20896, 21312, 21760, 22176, 22592, 23040, 23456, 23872, 24320, 24736, 25152, 25600, 26016, 26432, 26880, 27296, 27712, 28160, 28576, 28992, 29440, 29856, 
  //                                        30272, 30720, 31136, 31552, 32000, 32416, 32832, 33280, 33696, 34112, 34560, 34976, 35392, 35840, 36256, 36672, 37120, 37536, 37952, 38400, 38816, 39232, 39680, 
  //                                        40096, 40512, 40960, 41376, 41792, 42240, 42656 
  //                                        };

  extern unsigned int PowerGaugeValue_DecSUM_ArraySize;           // the number of elements in the array (zero indexed)
  extern unsigned int currentPowerGaugeValue_DecSUM_Array_Position;
  extern unsigned int currentPowerGaugeValue_DecSUM_Array_Value;
  extern unsigned int PowerGaugeValue_DecSUM_Array_Element;


  extern unsigned int TorqueGaugeValue_DecSUM_Array[];
  //                                        = {   
  //                                        3, 39, 79, 119, 159, 199, 235, 275, 315, 355, 395, 431, 471, 511, 551, 591, 627, 667, 707, 747, 787, 823, 863, 903, 943, 983, 
  //                                        1019, 1059, 1099, 1139, 1179, 1215, 1255, 1295, 1335, 1375, 1411, 1451, 1491, 1531, 1571, 1607, 1647, 1687, 1727, 1767, 1803, 1843, 1883, 1923, 1963, 1999, 
  //                                        2039, 2079, 2119, 2159, 2195, 2235, 2275, 2315, 2355, 2391, 2431, 2471, 2511, 2551, 2587, 2627, 2667, 2707, 2747, 2783, 2823, 2863, 2903, 2943, 2979, 
  //                                        3019, 3059, 3099, 3139, 3175, 3215, 3295, 3335, 3371, 3411, 3451, 3491, 3531, 3567, 3607, 3647, 3687, 3727, 3763, 3803, 3843, 3883, 3923
  //                                        };

  extern unsigned int TorqueGaugeValue_DecSUM_ArraySize;           // the number of elements in the array (zero indexed)
  extern unsigned int currentTorqueGaugeValue_DecSUM_Array_Position;
  extern unsigned int currentTorqueGaugeValue_DecSUM_Array_Value;
  extern unsigned int TorqueGaugeValue_DecSUM_Array_Element;

#pragma endregion

#pragma region // OBD-CAN Variables (primarily Calculated Integers variables read from OBD Port sniffing)

extern int OBD_CAN_VIN; // service09 0A
extern int OBD_CAN_CALID; // service09 0A
extern int OBD_CAN_ECUName; // service09 0A

extern int PT_CAN_VIN; // service09 0A
extern int PT_CAN_CALID; // service09 0A
extern int PT_CAN_ECUName; // service09 0A

extern unsigned int TFT_VehicleIdentificationComplete;

extern unsigned int OBD_CAN_STAT_FC_01;  // Flow Control request status
extern unsigned int OBD_CAN_STAT_FC_03;  // Flow Control request status
extern unsigned int OBD_CAN_STAT_FC_04;  // Flow Control request status
extern unsigned int OBD_CAN_STAT_FC_09;  // Flow Control request status
extern unsigned int OBD_CAN_STAT_FC_22_0x7DC;  // Flow Control request status
extern unsigned int OBD_CAN_STAT_FC_22_0x7E8;  // Flow Control request status
extern unsigned int OBD_CAN_STAT_FC_22_0x7E9;  // Flow Control request status
extern unsigned int OBD_CAN_STAT_FC_22_0x779;  // Flow Control request status
extern unsigned int OBD_CAN_STAT_FC_22_0x77D;  // Flow Control request status
extern unsigned int OBD_CAN_STAT_FC_22_0x700;
extern unsigned int OBD_CAN_Susp_0x7DC_STAT_FC_22;

;
extern unsigned int OBD_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22;
extern unsigned int OBD_CAN_TCU_0x7E9_TorqueActuals_STAT_FC_22;
extern unsigned int OBD_CAN_TCU_0x7E9_TorqueTargets_STAT_FC_22;

extern unsigned int OBD_CAN_TCU_0x7E9_PressureActuals_CF_Max;
extern unsigned int OBD_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22;
extern unsigned int OBD_CAN_TCU_0x7E9_PressureActuals_seq_Num;
extern unsigned int OBD_CAN_TCU_0x7E9_PressureActuals_CF_Num;
extern unsigned int OBD_CAN_TCU_0x7E9_PressureActuals_f_sid22_multi;

extern unsigned int OBD_CAN_f_sid22_TCU_PressureActuals_multi;


///////// ---- NEW Consolidated TCU Set 1 - Both Tgts and Actuals of Pressures and Torques

extern unsigned int OBD_CAN_TCU_0x7E9_AllTqsAndPressures_STAT_FC_22;  // Flow Control request status
extern unsigned int OBD_CAN_TCU_0x7E9_AllTqsAndPressures_CF_Num;
extern unsigned int OBD_CAN_TCU_0x7E9_AllTqsAndPressures_CF_Max;
extern unsigned int OBD_CAN_TCU_0x7E9_AllTqsAndPressures_seq_Num;
extern unsigned int OBD_CAN_TCU_0x7E9_AllTqsAndPressures_f_sid22_multi;
extern unsigned int OBD_CAN_TCU_0x7E9_AllTqsAndPressures_NextInitialReqReady;
extern unsigned int OBD_CAN_TCU_0x7E9_AllTqsAndPressures_IsRunning;


extern elapsedMillis loopDelayTCU_AllTqsAndPressures_Tx_1;
extern elapsedMillis loopDelayTCU_AllTqsAndPressures_Tx_2;
extern elapsedMillis loopDelayTCU_AllTqsAndPressures_Tx_3;
extern elapsedMillis loopDelayTCU_AllTqsAndPressures_Tx_4;

extern elapsedMillis loopDelayPT_CAN_TCU_ReqSet3_ClutchPlateTempActuals;
extern elapsedMillis loopDelayOBD_CAN_TCU_ReqSet3_ClutchPlateTempActuals;

extern elapsedMillis TimeSinceSTATFC1_PT_CAN_AllTqsAndPressures;
extern elapsedMillis TimeSinceSTATFC2_PT_CAN_AllTqsAndPressures;
extern elapsedMillis TimeSinceSTATFC3_PT_CAN_AllTqsAndPressures;
extern elapsedMillis TimeSinceSTATFC4_PT_CAN_AllTqsAndPressures;

extern elapsedMillis TimeSinceSTATFC1_OBD_CAN_AllTqsAndPressures;
extern elapsedMillis TimeSinceSTATFC2_OBD_CAN_AllTqsAndPressures;
extern elapsedMillis TimeSinceSTATFC3_OBD_CAN_AllTqsAndPressures;
extern elapsedMillis TimeSinceSTATFC4_OBD_CAN_AllTqsAndPressures;

extern elapsedMillis loopDelayTCU_ReqSet3_ClutchPlateTempActuals;

extern unsigned int TCU_NewSet1_FC_Running;




///////// ---- NEW Consolidated TCU Set 3 - 3 Temps

extern unsigned int OBD_CAN_TCU_0x7E9_Set3Temps_STAT_FC_22;  
extern unsigned int OBD_CAN_TCU_0x7E9_Set3Temps_CF_Num;
extern unsigned int OBD_CAN_TCU_0x7E9_Set3Temps_CF_Max;
extern unsigned int OBD_CAN_TCU_0x7E9_Set3Temps_seq_Num;
extern unsigned int OBD_CAN_TCU_0x7E9_Set3Temps_f_sid22_multi;
extern unsigned int OBD_CAN_TCU_0x7E9_Set3Temps_NextInitialReqReady;
extern unsigned int OBD_CAN_TCU_0x7E9_Set3Temps_IsRunning;


extern elapsedMillis loopDelayTCU_Set3Temps_Tx_1;
extern elapsedMillis loopDelayTCU_Set3Temps_Tx_2;
extern elapsedMillis loopDelayTCU_Set3Temps_Tx_3;
extern elapsedMillis loopDelayTCU_Set3Temps_Tx_4;

extern elapsedMillis TimeSinceSTATFC1_Set3Temps;
extern elapsedMillis TimeSinceSTATFC2_Set3Temps;
extern elapsedMillis TimeSinceInitialReq_TCUSet3Temps;
extern elapsedMillis TimesinceFCFinalSent_TCUSet3Temps;



// Instrument Cluster/MFSW
extern unsigned int OBD_CAN_MFSW_0x77A_STAT_FC_22;  // Flow Control request status
extern unsigned int OBD_CAN_MFSW_0x77A_CF_Num;
extern unsigned int OBD_CAN_MFSW_0x77A_CF_Max;
extern unsigned int OBD_CAN_MFSW_0x77A_seq_Num;
extern unsigned int OBD_CAN_MFSW_0x77A_f_sid22_multi;


extern unsigned int OBD_CAN_disp_mode;
extern unsigned int OBD_CAN_seq_Num;
extern unsigned int OBD_CAN_CF_Num;
extern unsigned int OBD_CAN_CF_Max;
 
extern unsigned int OBD_CAN_sid09inftyp;
extern unsigned int OBD_CAN_f_sid09VIN_multi;
extern unsigned int OBD_CAN_f_sid09CALID_multi;
extern unsigned int OBD_CAN_f_sid09ECUName_multi;
extern unsigned int OBD_CAN_f_sid22LvlSns_multi;
extern unsigned int OBD_CAN_f_sid22Haldex1_multi;
extern unsigned int OBD_CAN_f_sid22_0x7E8_multi;
extern unsigned int OBD_CAN_f_sid22_0x77D_multi;


// Suspension/Ride Height Sensors

extern int OBD_CAN_FL_Height; // Front Left Ride Height (mm)
extern int OBD_CAN_FR_Height; // Front Right Ride Height (mm)
extern int OBD_CAN_RL_Height; // Rear Left Ride Height (mm)
extern int OBD_CAN_RR_Height; // Rear Right Ride Height (mm)

extern unsigned int UnsignedLevelSensor_FL;
extern unsigned int UnsignedLevelSensor_FR;
extern unsigned int UnsignedLevelSensor_RL;
extern unsigned int UnsignedLevelSensor_RR;
 
extern unsigned int UnsignedLevelSensor_withOffset_FL;
extern unsigned int UnsignedLevelSensor_withOffset_FR;
extern unsigned int UnsignedLevelSensor_withOffset_RL;
extern unsigned int UnsignedLevelSensor_withOffset_RR;

extern unsigned int HaldexVoltageInt;
extern unsigned int HaldexPWMSigInt;

extern float HaldexPWMSig;
extern float HaldexVoltage;



#pragma region // OBD Service22 Variables from TCU - OBD

// DQ500 Pressures ---------------------------------------------------------------------

extern unsigned int OBD_CAN_msg0x7E9_Clutch1_PressureTarget_Raw, OBD_CAN_msg0x7E9_Clutch2_PressureTarget_Raw;
extern unsigned int OBD_CAN_msg0x7E9_Clutch1_PressureActual_Raw, OBD_CAN_msg0x7E9_Clutch2_PressureActual_Raw;

extern unsigned int OBD_CAN_msg0x7E9_Clutch1_PressureTarget_Int, OBD_CAN_msg0x7E9_Clutch2_PressureTarget_Int;
extern unsigned int OBD_CAN_msg0x7E9_Clutch1_PressureActual_Int, OBD_CAN_msg0x7E9_Clutch2_PressureActual_Int;

extern unsigned int OBD_CAN_TCU_Clutch1_PressureTarget_Int, OBD_CAN_TCU_Clutch2_PressureTarget_Int;
extern unsigned int OBD_CAN_TCU_Clutch1_PressureActual_Int, OBD_CAN_TCU_Clutch2_PressureActual_Int;

extern unsigned int OBD_CAN_TCU_OLD_Clutch1_PressureActual_Int, OBD_CAN_TCU_OLD_Clutch2_PressureActual_Int;

extern unsigned int OBD_CAN_TCU_OLD_DQ500_OilPressureActual_Int;
extern unsigned int PT_CAN_TCU_OLD_DQ500_OilPressureActual_Int;

extern unsigned int OBD_CAN_msg0x7E9_DQ500_OilPressureActual_Raw;
extern unsigned int OBD_CAN_msg0x7E9_DQ500_OilPressureTarget_Raw;

extern unsigned int OBD_CAN_msg0x7E9_DQ500_OilPressureActual_Int;
extern unsigned int OBD_CAN_msg0x7E9_DQ500_OilPressureTarget_Int;



extern unsigned int OBD_CAN_TCU_DQ500_OilPressureActual_Int;

// DQ500 Torques ---------------------------------------------------------------------

extern signed int OBD_CAN_msg0x7E9_Clutch1_TorqueTarget_Raw, OBD_CAN_msg0x7E9_Clutch2_TorqueTarget_Raw;
extern signed int OBD_CAN_msg0x7E9_Clutch1_TorqueActual_Raw, OBD_CAN_msg0x7E9_Clutch2_TorqueActual_Raw;

extern int OBD_CAN_msg0x7E9_Clutch1_TorqueTarget_Int, OBD_CAN_msg0x7E9_Clutch2_TorqueTarget_Int;
extern int OBD_CAN_msg0x7E9_Clutch1_TorqueActual_Int, OBD_CAN_msg0x7E9_Clutch2_TorqueActual_Int;

extern int OBD_CAN_TCU_Clutch1_TorqueTarget_Int, OBD_CAN_TCU_Clutch2_TorqueTarget_Int;
extern int OBD_CAN_TCU_Clutch1_TorqueActual_Int, OBD_CAN_TCU_Clutch2_TorqueActual_Int;

extern int OBD_CAN_TCU_DQ500_DriverIntentTq_Raw;
extern int OBD_CAN_TCU_DQ500_DriverIntentTq_Int;

extern int OBD_CAN_TCU_DQ500_IndicatedEngTq_Raw;
extern int OBD_CAN_TCU_DQ500_IndicatedEngTq_Int;


// DQ500 Temps ---------------------------------------------------------------------

extern int OBD_CAN_msg0x7E9_Clutch1_TempActual_Raw, OBD_CAN_msg0x7E9_Clutch2_TempActual_Raw;
extern int OBD_CAN_msg0x7E9_Clutch1_TempActual_Int, OBD_CAN_msg0x7E9_Clutch2_TempActual_Int;
extern int OBD_CAN_TCU_Clutch1_TempActual_Int, OBD_CAN_TCU_Clutch2_TempActual_Int;

extern int OBD_CAN_msg0x7E9_DQ500_CenterPlateTempActual_Raw;
extern int OBD_CAN_msg0x7E9_DQ500_CenterPlateTempActual_Int;
extern int OBD_CAN_TCU_DQ500_CenterPlateTempActual_Int;


#pragma endregion

#pragma region // OBD Service22 Variables from TCU - PT_CAN

// DQ500 via PT_CAN ---------------------------------------------------------------------

extern unsigned int PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22;
extern unsigned int PT_CAN_TCU_0x7E9_TorqueActuals_STAT_FC_22;
extern unsigned int PT_CAN_TCU_0x7E9_TorqueTargets_STAT_FC_22;

extern unsigned int PT_CAN_TCU_0x7E9_PressureActuals_CF_Max;
extern unsigned int PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22;
extern unsigned int PT_CAN_TCU_0x7E9_PressureActuals_seq_Num;
extern unsigned int PT_CAN_TCU_0x7E9_PressureActuals_CF_Num;
extern unsigned int PT_CAN_TCU_0x7E9_PressureActuals_f_sid22_multi;

extern unsigned int PT_CAN_f_sid22_TCU_PressureActuals_multi;

// DQ500 Pressures ---------------------------------------------------------------------


extern unsigned int PT_CAN_msg0x7E9_Clutch1_PressureTarget_Raw, PT_CAN_msg0x7E9_Clutch2_PressureTarget_Raw;
extern unsigned int PT_CAN_msg0x7E9_Clutch1_PressureActual_Raw, PT_CAN_msg0x7E9_Clutch2_PressureActual_Raw;

extern unsigned int PT_CAN_msg0x7E9_Clutch1_PressureTarget_Int, PT_CAN_msg0x7E9_Clutch2_PressureTarget_Int;
extern unsigned int PT_CAN_msg0x7E9_Clutch1_PressureActual_Int, PT_CAN_msg0x7E9_Clutch2_PressureActual_Int;

extern unsigned int PT_CAN_TCU_Clutch1_PressureTarget_Int, PT_CAN_TCU_Clutch2_PressureTarget_Int;
extern unsigned int PT_CAN_TCU_Clutch1_PressureActual_Int, PT_CAN_TCU_Clutch2_PressureActual_Int;

extern unsigned int PT_CAN_TCU_OLD_Clutch1_PressureActual_Int, PT_CAN_TCU_OLD_Clutch2_PressureActual_Int;

extern unsigned int PT_CAN_msg0x7E9_DQ500_OilPressureActual_Int;

extern unsigned int PT_CAN_msg0x7E9_DQ500_OilPressureTarget_Raw;
extern unsigned int PT_CAN_msg0x7E9_DQ500_OilPressureTarget_Int;

extern unsigned int PT_CAN_TCU_DQ500_OilPressureActual_Int;
extern unsigned int PT_CAN_TCU_OLD_DQ500_OilPressureActual_Int;


// DQ500 Torques ---------------------------------------------------------------------

extern signed int PT_CAN_msg0x7E9_Clutch1_TorqueTarget_Raw, PT_CAN_msg0x7E9_Clutch2_TorqueTarget_Raw;
extern signed int PT_CAN_msg0x7E9_Clutch1_TorqueActual_Raw, PT_CAN_msg0x7E9_Clutch2_TorqueActual_Raw;

extern int PT_CAN_msg0x7E9_Clutch1_TorqueTarget_Int, PT_CAN_msg0x7E9_Clutch2_TorqueTarget_Int;
extern int PT_CAN_msg0x7E9_Clutch1_TorqueActual_Int, PT_CAN_msg0x7E9_Clutch2_TorqueActual_Int;

extern int PT_CAN_TCU_Clutch1_TorqueTarget_Int, PT_CAN_TCU_Clutch2_TorqueTarget_Int;
extern int PT_CAN_TCU_Clutch1_TorqueActual_Int, PT_CAN_TCU_Clutch2_TorqueActual_Int;

extern int PT_CAN_TCU_DQ500_DriverIntentTq_Raw;
extern int PT_CAN_TCU_DQ500_DriverIntentTq_Int;

extern int PT_CAN_TCU_DQ500_IndicatedEngTq_Raw;
extern int PT_CAN_TCU_DQ500_IndicatedEngTq_Int;


// DQ500 Temps ---------------------------------------------------------------------

extern int PT_CAN_msg0x7E9_Clutch1_TempActual_Raw, PT_CAN_msg0x7E9_Clutch2_TempActual_Raw;
extern int PT_CAN_msg0x7E9_Clutch1_TempActual_Int, PT_CAN_msg0x7E9_Clutch2_TempActual_Int;
extern int PT_CAN_TCU_Clutch1_TempActual_Int, PT_CAN_TCU_Clutch2_TempActual_Int;

extern int PT_CAN_msg0x7E9_DQ500_CenterPlateTempActual_Raw;
extern int PT_CAN_msg0x7E9_DQ500_CenterPlateTempActual_Int;
extern int PT_CAN_TCU_DQ500_CenterPlateTempActual_Int;


#pragma endregion




#pragma region // OBD2 Service22 Variables from Multifunction Steering Wheel (MFSW)

 extern unsigned int MFSW_Default_NoButtonsPressed;

 extern unsigned int MFSW_LeftSide_TopLeftArrow_Pressed;
 extern unsigned int MFSW_LeftSide_TopRightArrow_Pressed;
 extern unsigned int MFSW_LeftSide_LowerLeftArrow_Pressed;
 extern unsigned int MFSW_LeftSide_LowerRightArrow_Pressed;

 extern unsigned int MFSW_RightSide_TopLeftArrow_Pressed;
 extern unsigned int MFSW_RightSide_TopRightArrow_Pressed;
 extern unsigned int MFSW_RightSide_LowerLeftArrow_Pressed;
 extern unsigned int MFSW_RightSide_LowerRightArrow_Pressed;

  // ---------------------------------------------------------------------

 extern unsigned int MFSW_LeftSide_BACK_Pressed;
 extern unsigned int MFSW_LeftSide_VIEW_Pressed;
 extern unsigned int MFSW_LeftSide_OK_Pressed;

 extern unsigned int MFSW_RightSide_Asterisk_Pressed;
  // ---------------------------------------------------------------------

 extern unsigned int MFSW_LeftSide_RotaryWheel_UP_1Click;
 extern unsigned int MFSW_LeftSide_RotaryWheel_UP_2Clicks;
 extern unsigned int MFSW_LeftSide_RotaryWheel_UP_3Clicks;

 extern unsigned int MFSW_LeftSide_RotaryWheel_DOWN_1Click;
 extern unsigned int MFSW_LeftSide_RotaryWheel_DOWN_2Clicks;
 extern unsigned int MFSW_LeftSide_RotaryWheel_DOWN_3Clicks;

 extern unsigned int MFSW_LeftSide_RotaryWheel_OK_Pressed;

  // ---------------------------------------------------------------------

 extern unsigned int MFSW_RightSide_RotaryWheel_UP_1Click;
 extern unsigned int MFSW_RightSide_RotaryWheel_UP_2Clicks;
 extern unsigned int MFSW_RightSide_RotaryWheel_UP_3Clicks;

 extern unsigned int MFSW_RightSide_RotaryWheel_DOWN_1Click;
 extern unsigned int MFSW_RightSide_RotaryWheel_DOWN_2Clicks;
 extern unsigned int MFSW_RightSide_RotaryWheel_DOWN_3Clicks;

 extern unsigned int MFSW_RightSide_RotaryWheel_MUTE_Pressed;

      // ---------------------------------------------------------------------

  extern unsigned int cruiseStalk_OBD_Raw;
  extern unsigned int cruiseStalk_UP_OBD;
  extern unsigned int cruiseStalk_DOWN_OBD;
  extern unsigned int CruiseStalk_RES_OBD;
  extern unsigned int cruiseStalk_FORWARD_OBD;
  extern unsigned int cruiseStalk_SET_OBD;

  extern unsigned int cruiseStalk_OBD_Service22;
  extern unsigned int cruiseStalk_OBD_Service22_shared_canSniff;

      // ---------------------------------------------------------------------



#pragma endregion



#pragma endregion

#pragma region // CAN Byte Coordinations - Bit shifted for sending from TFT to PT_CAN


// Analog Sensors
extern unsigned char lsbA0, msbA0, lsbA1, msbA1, lsbA2, msbA2, lsbA3, msbA3, lsbA8, msbA8, lsbA9, msbA9, lsbA10, msbA10;
extern unsigned char lsbA11, msbA11,  lsbA12, msbA12,  lsbA13, msbA13,  lsbA14, msbA14,  lsbA15, msbA15,  lsbA16, msbA16,  lsbA17, msbA17;

// NTC Thermistors

extern unsigned char lsbThermistor1C, msbThermistor1C;
extern unsigned char lsbThermistor2C, msbThermistor2C;

// Digital/Turbo Speed Sensors
extern unsigned char lsbturboRPM1, msbturboRPM1, lsbturboshaftRPM1raw, msbturboshaftRPM1raw;
extern unsigned char lsbturboRPM2, msbturboRPM2, lsbturboshaftFreqRaw, msbturboshaftFreqRaw;

// Internal Temperature and MCU
extern unsigned char lsbInternalCPUTemp, lsbIgnitionStatus, msbIgnitionStatus, lsbIgnitionAwake;

// DS1 Gauge Displays
extern unsigned char lsbDisplayGaugePVal, msbDisplayGaugePVal;
extern unsigned char lsbDisplayGaugeTVal, msbDisplayGaugeTVal;

// Direct Instrument Cluster Overrides

extern unsigned int DisplayGaugePVal_View;
extern unsigned int DisplayGaugeTVal_View;

extern unsigned char lsbDisplayGaugePVal_Direct, msbDisplayGaugePVal_Direct;
extern unsigned char lsbDisplayGaugeTVal_Direct, msbDisplayGaugeTVal_Direct;

extern unsigned char lsbDisplayGaugePVal_Direct__0x147_byte6_bits0to3, msbDisplayGaugePVal_Direct_0x147_byte7_Full;
extern unsigned char lsbDisplayGaugeTVal_Direct_0x147_byte5_Full, msbDisplayGaugeTVal_Direct_0x147_byte6_bits4to7;

extern unsigned char lsbMQB_TachEngineRPM_Override_0x107, msbMQB_TachEngineRPM_Override_0x107;

// IMU Variables
extern unsigned char lsbyaw, msbyaw, lsbpitch, msbpitch, lsbroll, msbroll;
extern unsigned char lsbshiftedAccelXms, msbshiftedAccelXms, lsbshiftedAccelYms, msbshiftedAccelYms, lsbshiftedAccelZms, msbshiftedAccelZms;
extern unsigned char lsbshiftedGyroXms, msbshiftedGyroXms, lsbshiftedGyroYms, msbshiftedGyroYms, lsbshiftedGyroZms, msbshiftedGyroZms;
extern unsigned char lsbdisplayAccelX, msbdisplayAccelX, lsbdisplayAccelY, msbdisplayAccelY;

// OBD-CAN Variables - Suspension, Haldex, etc
extern unsigned char lsbOffset_LevelSensor_FL, msbOffset_LevelSensor_FL, lsbOffset_LevelSensor_FR, msbOffset_LevelSensor_FR, lsbOffset_LevelSensor_RL, msbOffset_LevelSensor_RL, lsbOffset_LevelSensor_RR, msbOffset_LevelSensor_RR;
extern unsigned char lsbUnsignedLevelSensor_withOffset_FL, msbUnsignedLevelSensor_withOffset_FL, lsbUnsignedLevelSensor_withOffset_FR, msbUnsignedLevelSensor_withOffset_FR, lsbUnsignedLevelSensor_withOffset_RL, msbUnsignedLevelSensor_withOffset_RL, lsbUnsignedLevelSensor_withOffset_RR, msbUnsignedLevelSensor_withOffset_RR;
extern unsigned char lsbUnsignedLevelSensor_FL, msbUnsignedLevelSensor_FL, lsbUnsignedLevelSensor_FR, msbUnsignedLevelSensor_FR, lsbUnsignedLevelSensor_RL, msbUnsignedLevelSensor_RL, lsbUnsignedLevelSensor_RR, msbUnsignedLevelSensor_RR;
extern unsigned char lsbHaldexCurrentInt, msbHaldexCurrentInt, lsbHaldexVoltageInt, msbHaldexVoltageInt, lsbHaldexPWMSigInt, msbHaldexPWMSigInt;
extern unsigned char lsbHaldexClutchTemp, msbHaldexClutchTemp, lsbHaldexCoolingFinTemp, msbHaldexCoolingFinTemp, lsbHaldexDurationOverTemp, msbHaldexDurationOverTemp;

// Car Configuration variables
extern unsigned char lsbMQB_ConfigurationMode_JobNumber_to_Update;
extern unsigned char lsbCarConfig_SuspLevelSensors;
extern unsigned char lsbCarConfig_AWDController;
extern unsigned char lsbCarConfig_DisplayGaugesOverride;


extern int HaldexClutchTemp;
extern int HaldexCoolingFinTemp;

// Service22 CAN Variables - TCU

  extern byte OBD_CAN_msg0x7E9_Clutch1_PressureActual_LSB, OBD_CAN_msg0x7E9_Clutch1_PressureActual_MSB, OBD_CAN_msg0x7E9_Clutch2_PressureActual_LSB, OBD_CAN_msg0x7E9_Clutch2_PressureActual_MSB,  OBD_CAN_msg0x7E9_DQ500_OilPressureActual_LSB, OBD_CAN_msg0x7E9_DQ500_OilPressureActual_MSB;
  extern byte OBD_CAN_msg0x7E9_Clutch1_PressureTarget_LSB, OBD_CAN_msg0x7E9_Clutch1_PressureTarget_MSB, OBD_CAN_msg0x7E9_Clutch2_PressureTarget_LSB, OBD_CAN_msg0x7E9_Clutch2_PressureTarget_MSB,  OBD_CAN_msg0x7E9_DQ500_OilPressureTarget_LSB, OBD_CAN_msg0x7E9_DQ500_OilPressureTarget_MSB;
  extern byte OBD_CAN_msg0x7E9_Clutch1_TempActual_LSB, OBD_CAN_msg0x7E9_Clutch1_TempActual_MSB, OBD_CAN_msg0x7E9_Clutch2_TempActual_LSB, OBD_CAN_msg0x7E9_Clutch2_TempActual_MSB,  OBD_CAN_msg0x7E9_DQ500_CenterPlateTempActual_LSB, OBD_CAN_msg0x7E9_DQ500_CenterPlateTempActual_MSB;

  extern byte OBD_CAN_msg0x7E9_Clutch1_TorqueActual_LSB, OBD_CAN_msg0x7E9_Clutch1_TorqueActual_MSB, OBD_CAN_msg0x7E9_Clutch2_TorqueActual_LSB, OBD_CAN_msg0x7E9_Clutch2_TorqueActual_MSB;
  extern byte OBD_CAN_msg0x7E9_Clutch1_TorqueTarget_LSB, OBD_CAN_msg0x7E9_Clutch1_TorqueTarget_MSB, OBD_CAN_msg0x7E9_Clutch2_TorqueTarget_LSB, OBD_CAN_msg0x7E9_Clutch2_TorqueTarget_MSB, OBD_CAN_msg0x7E9_DQ500_DriverIntentTq_LSB, OBD_CAN_msg0x7E9_DQ500_DriverIntentTq_MSB;




  extern unsigned char lsb_OBD_CAN_TCU_Clutch1_PressureActual_Int, msb_OBD_CAN_TCU_Clutch1_PressureActual_Int;
  extern unsigned char lsb_OBD_CAN_TCU_Clutch2_PressureActual_Int, msb_OBD_CAN_TCU_Clutch2_PressureActual_Int;
  extern unsigned char lsb_OBD_CAN_TCU_DQ500_OilPressureActual_Int, msb_OBD_CAN_TCU_DQ500_OilPressureActual_Int;

  extern unsigned char lsb_PT_CAN_TCU_Clutch1_PressureActual_Int, msb_PT_CAN_TCU_Clutch1_PressureActual_Int;
  extern unsigned char lsb_PT_CAN_TCU_Clutch2_PressureActual_Int, msb_PT_CAN_TCU_Clutch2_PressureActual_Int;
  extern unsigned char lsb_PT_CAN_TCU_DQ500_OilPressureActual_Int, msb_PT_CAN_TCU_DQ500_OilPressureActual_Int;


  extern unsigned char lsb_OBD_CAN_TCU_Clutch1_PressureTarget_Int, msb_OBD_CAN_TCU_Clutch1_PressureTarget_Int;
  extern unsigned char lsb_OBD_CAN_TCU_Clutch2_PressureTarget_Int, msb_OBD_CAN_TCU_Clutch2_PressureTarget_Int;
  extern unsigned char lsb_OBD_CAN_TCU_DQ500_OilPressureTarget_Int, msb_OBD_CAN_TCU_DQ500_OilPressureTarget_Int;

  extern unsigned char lsb_OBD_CAN_TCU_Clutch1_TempActual_Int, msb_OBD_CAN_TCU_Clutch1_TempActual_Int;
  extern unsigned char lsb_OBD_CAN_TCU_Clutch2_TempActual_Int, msb_OBD_CAN_TCU_Clutch2_TempActual_Int;
  extern unsigned char lsb_OBD_CAN_TCU_DQ500_CenterPlateTempActual_Int, msb_OBD_CAN_TCU_DQ500_CenterPlateTempActual_Int;


  extern unsigned char lsb_OBD_CAN_TCU_Clutch1_TorqueActual_Int, msb_OBD_CAN_TCU_Clutch1_TorqueActual_Int;
  extern unsigned char lsb_OBD_CAN_TCU_Clutch2_TorqueActual_Int, msb_OBD_CAN_TCU_Clutch2_TorqueActual_Int;
  extern unsigned char lsb_OBD_CAN_TCU_DQ500_DriverIntentTq_Int, msb_OBD_CAN_TCU_DQ500_DriverIntentTq_Int;
  extern unsigned char lsb_OBD_CAN_TCU_DQ500_IndicatedEngTq_Int, msb_OBD_CAN_TCU_DQ500_IndicatedEngTq_Int;

  extern unsigned char lsb_OBD_CAN_TCU_Clutch1_TorqueTarget_Int, msb_OBD_CAN_TCU_Clutch1_TorqueTarget_Int;
  extern unsigned char lsb_OBD_CAN_TCU_Clutch2_TorqueTarget_Int, msb_OBD_CAN_TCU_Clutch2_TorqueTarget_Int;

  extern unsigned char lsb_xxx_CAN_TCU_Clutch1_TorqueTarget_Int, msb_xxx_CAN_TCU_Clutch1_TorqueTarget_Int;
  extern unsigned char lsb_xxx_CAN_TCU_Clutch2_TorqueTarget_Int, msb_xxx_CAN_TCU_Clutch2_TorqueTarget_Int;
  extern unsigned char lsb_xxx_CAN_TCU_DQ500_DriverIntentTq_Int, msb_xxx_CAN_TCU_DQ500_DriverIntentTq_Int;



// PT-CAN Variables - ECU/TCU/Car

  extern unsigned char lsbMQB_AutoUPshift_Gear1to2_RPMSetpoint_Int, msbMQB_AutoUPshift_Gear1to2_RPMSetpoint_Int;
  extern unsigned char lsbMQB_AutoUPshift_Gear2to3_RPMSetpoint_Int, msbMQB_AutoUPshift_Gear2to3_RPMSetpoint_Int;
  extern unsigned char lsbMQB_AutoUPshift_Gear3to4_RPMSetpoint_Int, msbMQB_AutoUPshift_Gear3to4_RPMSetpoint_Int;
  extern unsigned char lsbMQB_AutoUPshift_Gear4to5_RPMSetpoint_Int, msbMQB_AutoUPshift_Gear4to5_RPMSetpoint_Int;
  extern unsigned char lsbMQB_AutoUPshift_Gear5to6_RPMSetpoint_Int, msbMQB_AutoUPshift_Gear5to6_RPMSetpoint_Int;
  extern unsigned char lsbMQB_AutoUPshift_Gear6to7_RPMSetpoint_Int, msbMQB_AutoUPshift_Gear6to7_RPMSetpoint_Int;

  extern unsigned char lsbMQB_AutoUPshift_Gear1to2_RPMSetpoint_Trigger_Int, msbMQB_AutoUPshift_Gear1to2_RPMSetpoint_Trigger_Int;
  extern unsigned char lsbMQB_AutoUPshift_Gear2to3_RPMSetpoint_Trigger_Int, msbMQB_AutoUPshift_Gear2to3_RPMSetpoint_Trigger_Int;
  extern unsigned char lsbMQB_AutoUPshift_Gear3to4_RPMSetpoint_Trigger_Int, msbMQB_AutoUPshift_Gear3to4_RPMSetpoint_Trigger_Int;
  extern unsigned char lsbMQB_AutoUPshift_Gear4to5_RPMSetpoint_Trigger_Int, msbMQB_AutoUPshift_Gear4to5_RPMSetpoint_Trigger_Int;
  extern unsigned char lsbMQB_AutoUPshift_Gear5to6_RPMSetpoint_Trigger_Int, msbMQB_AutoUPshift_Gear5to6_RPMSetpoint_Trigger_Int;
  extern unsigned char lsbMQB_AutoUPshift_Gear6to7_RPMSetpoint_Trigger_Int, msbMQB_AutoUPshift_Gear6to7_RPMSetpoint_Trigger_Int;

  extern unsigned char MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Intchar;


  extern unsigned char lsbMQB_AutoDOWNshift_Gear2to1_RPMSetpoint_Int, msbMQB_AutoDOWNshift_Gear2to1_RPMSetpoint_Int;
  extern unsigned char lsbMQB_AutoDOWNshift_Gear3to2_RPMSetpoint_Int, msbMQB_AutoDOWNshift_Gear3to2_RPMSetpoint_Int;
  extern unsigned char lsbMQB_AutoDOWNshift_Gear4to3_RPMSetpoint_Int, msbMQB_AutoDOWNshift_Gear4to3_RPMSetpoint_Int;
  extern unsigned char lsbMQB_AutoDOWNshift_Gear5to4_RPMSetpoint_Int, msbMQB_AutoDOWNshift_Gear5to4_RPMSetpoint_Int;
  extern unsigned char lsbMQB_AutoDOWNshift_Gear6to5_RPMSetpoint_Int, msbMQB_AutoDOWNshift_Gear6to5_RPMSetpoint_Int;
  extern unsigned char lsbMQB_AutoDOWNshift_Gear7to6_RPMSetpoint_Int, msbMQB_AutoDOWNshift_Gear7to6_RPMSetpoint_Int;


  extern unsigned char lsbMQB_LaunchControl_EngineRPM_Ramp_Setpoint_Int, msbMQB_LaunchControl_EngineRPM_Ramp_Setpoint_Int;
  extern unsigned char lsbMQB_LaunchControl_EngineRPM_Ramp_Setpoint_Int_8bit, msbMQB_LaunchControl_EngineRPM_Ramp_Setpoint_Int_8bit;

  extern unsigned char lsbMQB_LaunchControl_EngineRPM_Hold_Setpoint_Int, msbMQB_LaunchControl_EngineRPM_Hold_Setpoint_Int;
  extern unsigned char lsbMQB_LaunchControl_EngineRPM_Hold_Setpoint_Int_8bit;

  extern unsigned char lsbMQB_LaunchControl_EngineRPM_Transition_Setpoint_Int, msbMQB_LaunchControl_EngineRPM_Transition_Setpoint_Int;
  extern unsigned char lsbMQB_LaunchControl_EngineRPM_Transition_Setpoint_Int_8bit;

  extern unsigned char lsbMQB_LaunchControl_EngineRPM_TransitionRateMultiplier_Int, msbMQB_LaunchControl_EngineRPM_TransitionRateMultiplier_Int;
  // extern unsigned char lsbMQB_LaunchControl_EngineRPM_Transition_Setpoint_Int_8bit;

  extern signed char lsbMQB_LaunchControl_EngineRPM_Setpoint_CurrentDelta, msbMQB_LaunchControl_EngineRPM_Setpoint_CurrentDelta;
  extern signed char lsbMQB_LaunchControl_EngineRPM_Setpoint_CurrentDelta_8bit;


  extern unsigned char lsbMQB_LaunchControl_EngineRPM_RAMP_Timeout_Int, msbMQB_LaunchControl_EngineRPM_RAMP_Timeout_Int;

  extern unsigned char lsbMQB_LaunchControl_EngineRPM_PID_BaseOffset_Int, msbMQB_LaunchControl_EngineRPM_PID_BaseOffset_Int; // this is in the range of 0-20 (a value of 30 is subtracted from this to arrive at the actual offset used )

  extern unsigned char lsbMQB_LaunchControl_EngineRPM_PID_Ki_Term_Int, msbMQB_LaunchControl_EngineRPM_PID_Ki_Term_Int;

  extern unsigned char lsbMQB_AutoShift_EngineRPM_KalmanFilteredMPC_Gain_Int, msbMQB_AutoShift_EngineRPM_KalmanFilteredMPC_Gain_Int;






  extern unsigned char lsbMQB_TFT_EngineRPM_Setpoint_Int, msbMQB_TFT_EngineRPM_Setpoint_Int;
  extern unsigned char lsbMQB_TFT_EngineRPM_Setpoint_Int_8bit;


  extern unsigned char lsbMQB_LaunchControl_BoostWindow_EntryThreshold_Achieved, msbMQB_LaunchControl_BoostWindow_EntryThreshold_Achieved;
  extern unsigned char lsbMQB_LaunchControl_BoostWindow_Maintained_Status, msbMQB_LaunchControl_BoostWindow_Maintained_Status;

  extern unsigned char lsbMQB_LaunchControl_BoostWindow_EntryThresholdPressure_Int, msbMQB_LaunchControl_BoostWindow_EntryThresholdPressure_Int;
  extern unsigned char lsbMQB_LaunchControl_BoostWindow_EntryThresholdPressure_Int_8bit, msbMQB_LaunchControl_BoostWindow_EntryThresholdPressure_Int_8bit;

  extern unsigned char lsbMQB_LaunchControl_BoostWindow_minMaintainPressure_Int, msbMQB_LaunchControl_BoostWindow_minMaintainPressure_Int;
  extern unsigned char lsbMQB_LaunchControl_BoostWindow_minMaintainPressure_Int_8bit, msbMQB_LaunchControl_BoostWindow_minMaintainPressure_Int_8bit;
  extern unsigned char lsbMQB_LaunchControl_BoostVariableSelection_Int, msbMQB_LaunchControl_BoostVariableSelection_Int;


  extern unsigned char lsbMQB_TractionControl_TgtSlipRate_Int, msbMQB_TractionControl_TgtSlipRate_Int;
  extern unsigned char lsbMQB_TractionControl_CorrectionAggression_Int, msbMQB_TractionControl_CorrectionAggression_Int;
  extern unsigned char lsbMQB_TractionControl_CorrectionAggression_Int_8bit;
  
  extern unsigned char lsbMQB_BurnoutMode_EngineRPM_Setpoint_Int, msbMQB_BurnoutMode_EngineRPM_Setpoint_Int;
  extern unsigned char lsbMQB_BurnoutMode_EngineRPM_Setpoint_Int_8bit, msbMQB_BurnoutMode_EngineRPM_Setpoint_Int_8bit;

  extern unsigned char lsbMQB_LaunchControl_CombinedSignalActive, msbMQB_LaunchControl_CombinedSignalActive;
  extern unsigned char lsbMQB_LaunchControl_CombinedSignalActive_8bit, msbMQB_LaunchControl_CombinedSignalActive_8bit;

  extern unsigned char lsbMQB_Parachute_ArmSpeed_Setpoint_Int, msbMQB_Parachute_ArmSpeed_Setpoint_Int;
  extern unsigned char lsbMQB_Parachute_ArmSpeed_Setpoint_Int_8bit;
  extern unsigned char lsbMQB_Parachute_PreArmedStatus;



  extern unsigned char lsbMQB_BoostByGearMultiplier_ActivationStatus_Int;
  extern unsigned char lsbMQB_BoostByGearMultiplier_Gear1_Int;
  extern unsigned char lsbMQB_BoostByGearMultiplier_Gear2_Int;
  extern unsigned char lsbMQB_BoostByGearMultiplier_Gear3_Int;
  extern unsigned char lsbMQB_BoostByGearMultiplier_Gear4_Int;
  extern unsigned char lsbMQB_BoostByGearMultiplier_Gear5_Int;
  extern unsigned char lsbMQB_BoostByGearMultiplier_Gear6_Int;
  extern unsigned char lsbMQB_BoostByGearMultiplier_Gear7_Int;

  extern unsigned char lsbMQB_BoostByGearMultiplier_Current;


  extern unsigned char lsbMQB_LS1_Output_Type;
  extern unsigned char lsbMQB_LS2_Output_Type;
  extern unsigned char lsbMQB_LS3_Output_Type;
  extern unsigned char lsbMQB_LS4_Output_Type;
  extern unsigned char lsbMQB_LS5_Output_Type;
  extern unsigned char lsbMQB_LS6_Output_Type;
  extern unsigned char lsbMQB_HS1_Output_Type;


  extern unsigned char lsbMQB_LS1_Output_ActivationTimer_Setpoint_ms, msbMQB_LS1_Output_ActivationTimer_Setpoint_ms;
  extern unsigned char lsbMQB_LS2_Output_ActivationTimer_Setpoint_ms, msbMQB_LS2_Output_ActivationTimer_Setpoint_ms;
  extern unsigned char lsbMQB_LS3_Output_ActivationTimer_Setpoint_ms, msbMQB_LS3_Output_ActivationTimer_Setpoint_ms;
  extern unsigned char lsbMQB_LS4_Output_ActivationTimer_Setpoint_ms, msbMQB_LS4_Output_ActivationTimer_Setpoint_ms;
  extern unsigned char lsbMQB_LS5_Output_ActivationTimer_Setpoint_ms, msbMQB_LS5_Output_ActivationTimer_Setpoint_ms;
  extern unsigned char lsbMQB_LS6_Output_ActivationTimer_Setpoint_ms, msbMQB_LS6_Output_ActivationTimer_Setpoint_ms;
  extern unsigned char lsbMQB_HS1_Output_ActivationTimer_Setpoint_ms, msbMQB_HS1_Output_ActivationTimer_Setpoint_ms;

  extern unsigned char lsbMQB_LS1_Output_PWMFreq;
  extern unsigned char lsbMQB_LS2_Output_PWMFreq;
  extern unsigned char lsbMQB_LS3_Output_PWMFreq;
  extern unsigned char lsbMQB_LS4_Output_PWMFreq;
  extern unsigned char lsbMQB_LS5_Output_PWMFreq;
  extern unsigned char lsbMQB_LS6_Output_PWMFreq;
  extern unsigned char lsbMQB_HS1_Output_PWMFreq;


  extern unsigned char lsbTemp7_Type;
  extern unsigned char lsbTemp8_Type;
   

#pragma endregion

#pragma region // External CAN Variables sniffed, originating from aftermarket ECUs/Controllers (Syvecs AWD, TFT, Fuel Pumps, widebands, etc)


  extern word SyvecsAWD_AWDBase_CAN;
  extern word SyvecsAWD_AWDFinal_CAN;
  extern word SyvecsAWD_AWDPot_CAN;
  extern word SyvecsAWD_AWDPotMultiplier_CAN;
  extern word SyvecsAWD_OutputCurrent;

  extern word SyvecsAWD_FrontSlipCalc_CAN;
  extern word SyvecsAWD_RearSlipCalc_CAN;
  extern word SyvecsAWD_CenterSlipCalc_CAN;
  extern word SyvecsAWD_FrontSlipAdder_CAN;
  extern word SyvecsAWD_RearSlipAdder_CAN;
  extern word SyvecsAWD_CenterSlipAdder_CAN;

  extern word SyvecsAWD_SteeringWheelAngle_CAN;
  extern word SyvecsAWD_SteeringWheelAngleAdder_CAN;
  extern word SyvecsAWD_SteeringWheelAngleMultiplier_CAN;

  extern word SyvecsAWD_BrakePressure_CAN;
  extern word SyvecsAWD_BrakePressureAdder_CAN;
  extern word SyvecsAWD_BrakePressureAdderMultiplier_CAN;


  // ----------------------------------------------------------

  extern unsigned char SyvecsAWD_AWDBase;
  extern unsigned char SyvecsAWD_AWDFinal;
  extern unsigned char SyvecsAWD_AWDPot;
  extern unsigned char SyvecsAWD_AWDPotMultiplier;

  extern unsigned char SyvecsAWD_FrontSlipCalc;
  extern unsigned char SyvecsAWD_RearSlipCalc;
  extern unsigned char SyvecsAWD_CenterSlipCalc;
  extern unsigned char SyvecsAWD_FrontSlipAdder;
  extern unsigned char SyvecsAWD_RearSlipAdder;
  extern unsigned char SyvecsAWD_CenterSlipAdder;


  extern unsigned char SyvecsAWD_SteeringWheelAngle;
  extern unsigned char SyvecsAWD_SteeringWheelAngleMultiplier;
  extern unsigned char SyvecsAWD_SteeringWheelAngleAdder;

  extern unsigned char SyvecsAWD_BrakePressure;
  extern unsigned char SyvecsAWD_BrakePressureMultiplier;
  extern unsigned char SyvecsAWD_BrakePressureAdder;



  // ----------------------------------------------------------


  extern unsigned int SyvecsAWD_AWDBase_Int;
  extern unsigned int SyvecsAWD_AWDFinal_Int;
  extern unsigned int SyvecsAWD_AWDPot_Int;
  extern unsigned int SyvecsAWD_AWDPotMultiplier_Int;


  extern unsigned int SyvecsAWD_FrontSlipCalc_Int;
  extern unsigned int SyvecsAWD_RearSlipCalc_Int;
  extern unsigned int SyvecsAWD_CenterSlipCalc_Int;
  extern unsigned int SyvecsAWD_FrontSlipAdder_Int;
  extern unsigned int SyvecsAWD_RearSlipAdder_Int;
  extern unsigned int SyvecsAWD_CenterSlipAdder_Int;



  extern unsigned int SyvecsAWD_SteeringWheelAngle_Int;
  extern unsigned int SyvecsAWD_SteeringWheelAngleMultiplier_Int;
  extern unsigned int SyvecsAWD_SteeringWheelAngleAdder_Int;

  extern unsigned int SyvecsAWD_BrakePressureBAR_Int;
  extern unsigned int SyvecsAWD_BrakePressureMultiplier_Int;
  extern unsigned int SyvecsAWD_BrakePressureAdder_Int;



  // ----------------------------------------------------------


  extern float SyvecsAWD_AWDBaseFloat;
  extern float SyvecsAWD_AWDFinalFloat;
  extern float SyvecsAWD_AWDPotFloat;
  extern float SyvecsAWD_AWDPotMultiplier_Float;

  extern float SyvecsAWD_FrontSlipCalc_Float;
  extern float SyvecsAWD_RearSlipCalc_Float;
  extern float SyvecsAWD_CenterSlipCalc_Float;
  extern float SyvecsAWD_FrontSlipAdder_Float;
  extern float SyvecsAWD_RearSlipAdder_Float;
  extern float SyvecsAWD_CenterSlipAdder_Float;

  extern float SyvecsAWD_SteeringWheelAngle_Float;
  extern float SyvecsAWD_SteeringWheelAngleMultiplier_Float;
  extern float SyvecsAWD_SteeringWheelAngleAdder_Float;

  extern float SyvecsAWD_BrakePressure_Float;
  extern float SyvecsAWD_BrakePressureMultiplier_Float;
  extern float SyvecsAWD_BrakePressureAdder_Float;


  // ----------------------------------------------------------







#pragma endregion

#pragma region // MITM Variables, Statuses, etc for communication between TFT Modules

  extern word TFT_MITM_LC_BumpIn_Status_Combined_CAN;
  extern word TFT_MITM_LC_BumpIn_Status_Armed_CAN;
  extern word TFT_MITM_LC_BumpIn_Status_Active_CAN;
  extern word TFT_MITM_LC_BumpInActive_RollingStatus_CAN;
  extern word TFT_MITM_LC_BumpIn_TimeSinceActiveMS_CAN;
  extern word TFT_MITM_LC_BumpIn_TrueWhlSpdRR_fromABS_CAN;
  extern word TFT_MITM_LC_BumpIn_TrueBrakePressureMPA_fromABS_CAN;


  extern unsigned int TFT_MITM_LC_BumpIn_Status_Combined_Int;
  extern unsigned int TFT_MITM_LC_BumpIn_Status_Armed_Int;
  extern unsigned int TFT_MITM_LC_BumpIn_Status_Active_Int;
  extern unsigned int TFT_MITM_LC_BumpInActive_RollingStatus_Int;

  extern float        TFT_MITM_LC_BumpIn_TrueWhlSpdRR_fromABS_Float;
  extern unsigned int TFT_MITM_LC_BumpIn_TrueWhlSpdRR_fromABS_8bit;
  extern unsigned int TFT_MITM_LC_BumpIn_TrueWhlSpdRR_fromABS_canTx_16bit;
  
  extern float        TFT_MITM_LC_BumpIn_TrueBrakePressureMPA_fromABS_Float;
  extern unsigned int TFT_MITM_LC_BumpIn_TrueBrakePressureMPA_fromABS_8bit;
  extern unsigned int TFT_MITM_LC_BumpIn_TrueBrakePressureMPA_fromABS_canTx_16bit;
  extern unsigned int TFT_MITM_LC_BumpIn_TimeSinceActiveMS;
  extern unsigned int TFT_MITM_LC_BumpIn_TimeSinceActiveMS_8bit;
  extern unsigned int TFT_MITM_LC_BumpIn_TimeSinceActiveMS_16bit;


  extern float TFT_MITM_TCU_EngTQ_Multiplier_Float;
  extern unsigned int TFT_MITM_TCU_EngTQ_Multiplier_Int;
  extern unsigned int TFT_MITM_TCU_EngTQ_Multiplier_Setting_Int;




#pragma endregion

#pragma region // SD Card Writing
extern int SDCardWriteConfig;
extern int SDCardWrite_Active;
extern elapsedMillis loopDelaySDCardWrite;

#pragma endregion





  #pragma region // 0xA7 - Motor_11 - Engine Torques
   extern uint8_t  MQB_Motor_11_0xA7_CRC;
   extern uint8_t  MQB_Motor_11_0xA7_BZ;
   extern int16_t  MQB_Motor_11_0xA7_EngineTqTargetRaw_0xA7;
   extern int16_t  MQB_Motor_11_0xA7_EngineTqActual_0xA7;
   extern int16_t  MQB_Motor_11_0xA7_EngineTotalMomentsInertia;
   extern int16_t  MQB_Motor_11_0xA7_EngineTqTargetFiltered_0xA7;
   extern int16_t  MQB_Motor_11_0xA7_EngineTqThrust;
   extern bool     MQB_Motor_11_0xA7_Status_NormalOperation_01;
   extern bool     MQB_Motor_11_0xA7_First_ImprecisionThreshold;
   extern bool     MQB_Motor_11_0xA7_QBit_EngineTq;

  extern bool EngineTorqueModification_0xA7_Active;
  extern bool EngineTorqueModification_0xA8_Active;

  extern signed long int motor11_EngineTqTargetRaw_Final;


  extern int EngineTqMultiplier_0xA7_Int; // This is divided by 100 to arrive at the actual multiplier value used (e.g. a value of 120 represents a 1.2 multiplier, a value of 80 represents a 0.8 multiplier, etc)
  extern int EngineTqMultiplier_0xA8_Int; // This is divided by 100 to arrive at the actual multiplier value used (e.g. a value of 120 represents a 1.2 multiplier, a value of 80 represents a 0.8 multiplier, etc)

  extern float EngineTqMultiplier_0xA7;
  extern float EngineTqMultiplier_0xA8;

  #pragma endregion



#endif