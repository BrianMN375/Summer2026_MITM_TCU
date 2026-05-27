#include <Arduino.h>
#include "checksum.h"
#include <FlexCAN_T4.h>
#include "checksum.h"
#include "MQB_CANbus_ParsingHelpers.h"
#include "global_vars.h"
// #include "dbc_autogen.hpp"


#pragma region // base FlexCAN Definitions


elapsedMillis TimeSinceIgnitionON;
static unsigned long seed_report_last_ms = 0;
const unsigned long SEED_REPORT_INTERVAL_MS = 60UL * 1000UL; // every 60s

static FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> TFTCAN1; // This is connected to the ABS Node from the interruption in the SuspCAN bus
static FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> TFTCAN2; // This is connected to the Gateway side of the interruption in the SuspCAN bus
static FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16> TFTCAN3; // This is just tapped to the PT_CAN 


#pragma endregion

#pragma region // Operational Variables, CAN Definitions, etc

// static const uint32_t IDsToWatch[] = { 0x106, 0x1AB, 0x101 };
static const uint32_t IDsToWatch[] = { 0x101, 0x104, 0x106, 0x116, 0x11E, 0x120, 0x1AB, 0xA7, 0xA8, 0x11E, 0xB2, 0xFD, 0x392, 0x31B };
// static const uint32_t IDsToWatch[] = {0x1AB };
static const size_t NumIDsToWatch = sizeof(IDsToWatch) / sizeof(IDsToWatch[0]);

static bool id_is_watched(uint32_t id) {
  for (size_t i = 0; i < NumIDsToWatch; i++)
    if (IDsToWatch[i] == id) return true;
  return false;
}



elapsedMillis StartupDelay;
elapsedMillis loopDelay_OBD_KeepAwake;
elapsedMillis loopDelay_OBD_Brake_Pressure_Req;
elapsedMillis loopDelay_OBD_Cruise_Stalk_Req;
elapsedMillis OBD_DTC_Clear_Msg2_Delay;

elapsedMillis loopDelaySerialPrint;
elapsedMillis OBDCruise_Stalk_Req;
elapsedMillis GenericCANtx_IntervalTimer;
elapsedMillis GenericCANtx_IncrementingTimer1;
elapsedMillis GenericCANtx_IncrementingTimer2;
elapsedMillis GenericCANtx_IncrementingTimer3;

elapsedMillis PT_CAN_canTxInterval_0x795;
elapsedMillis PT_CAN_canTxInterval_0x796;

elapsedMillis LC_BumpIn_TimeSinceActivation;
unsigned int LC_BumpIn_ActivationButton;
unsigned int LC_BumpIn_ActivationStatus;
unsigned int LC_BumpIn_MITM_Status;
unsigned int LC_BumpIn_AcceptableConditions;

  // ----------------------------------------------------------

// TFT MITM Statuses

#pragma region // MITM Variables and Statuses for communication between TFT Modules

  word TFT_MITM_LC_BumpIn_Status_Combined_CAN;
  word TFT_MITM_LC_BumpIn_Status_Armed_CAN;
  word TFT_MITM_LC_BumpIn_Status_Active_CAN;
  word TFT_MITM_LC_BumpInActive_RollingStatus_CAN;
  word TFT_MITM_LC_BumpIn_TimeSinceActiveMS_CAN;
  word TFT_MITM_LC_BumpIn_TrueWhlSpdRR_fromABS_CAN;
  word TFT_MITM_LC_BumpIn_TrueBrakePressureMPA_fromABS_CAN;

  unsigned int TFT_MITM_LC_BumpIn_Status_Combined_Int;
  unsigned int TFT_MITM_LC_BumpIn_Status_Armed_Int;
  unsigned int TFT_MITM_LC_BumpIn_Status_Active_Int;
  unsigned int TFT_MITM_LC_BumpInActive_RollingStatus_Int;

  unsigned int TFT_MITM_LC_BumpIn_TrueWhlSpdRR_fromABS_Int;
  float        TFT_MITM_LC_BumpIn_TrueWhlSpdRR_fromABS_Float;
  unsigned int TFT_MITM_LC_BumpIn_TrueWhlSpdRR_fromABS_8bit;
  unsigned int TFT_MITM_LC_BumpIn_TrueWhlSpdRR_fromABS_canTx_16bit;

  float        TFT_MITM_LC_BumpIn_TrueBrakePressureMPA_fromABS_Float;
  unsigned int TFT_MITM_LC_BumpIn_TrueBrakePressureMPA_fromABS_8bit;
  unsigned int TFT_MITM_LC_BumpIn_TrueBrakePressureMPA_fromABS_canTx_16bit;
  unsigned int TFT_MITM_LC_BumpIn_TimeSinceActiveMS;
  unsigned int TFT_MITM_LC_BumpIn_TimeSinceActiveMS_8bit;
  unsigned int TFT_MITM_LC_BumpIn_TimeSinceActiveMS_canTx_16bit;


  float TFT_MITM_TCU_EngTQ_Multiplier_Float;
  unsigned int TFT_MITM_TCU_EngTQ_Multiplier_Int;
  unsigned int TFT_MITM_TCU_EngTQ_Multiplier_Setting_Int;








#pragma endregion




unsigned int LC_BumpIn_0x101_CallbackRecorded;
unsigned int LC_BumpIn_0x104_CallbackRecorded;
unsigned int LC_BumpIn_0x106_CallbackRecorded;
unsigned int LC_BumpIn_0x116_CallbackRecorded;
unsigned int LC_BumpIn_0x11E_CallbackRecorded;
unsigned int LC_BumpIn_0x1AB_CallbackRecorded;
unsigned int LC_BumpIn_0x31B_CallbackRecorded;
unsigned int LC_BumpIn_0x392_CallbackRecorded;
unsigned int LC_BumpIn_0x3C8_CallbackRecorded;
unsigned int LC_BumpIn_0xB2_CallbackRecorded;
unsigned int LC_BumpIn_0xFD_CallbackRecorded;
// unsigned int LC_BumpIn_0xB2_CallbackRecorded;
// unsigned int LC_BumpIn_0xB2_CallbackRecorded;
// unsigned int LC_BumpIn_0xB2_CallbackRecorded;




unsigned int GenericCANtx_IncrementingTimer1_Int;
unsigned int GenericCANtx_IncrementingTimer2_Int;
unsigned int GenericCANtx_IncrementingTimer3_Int;


// #define NUM_TX_MAILBOXES 2
// #define NUM_RX_MAILBOXES 16



// CAN_message_t fwd; // (now declared within each loop_can_poll)

CAN_message_t msgx700;
CAN_message_t msgx7E1;

CAN_message_t msgx710;
CAN_message_t msgx200;

CAN_message_t PT_CAN_msg0xALL;
CAN_message_t Susp_CAN_msg0xALL;




word Susp_CAN_msg0xALL_buf0, Susp_CAN_msg0xALL_buf1, Susp_CAN_msg0xALL_buf2, Susp_CAN_msg0xALL_buf3, Susp_CAN_msg0xALL_buf4, Susp_CAN_msg0xALL_buf5, Susp_CAN_msg0xALL_buf6, Susp_CAN_msg0xALL_buf7;

  CAN_message_t msg1;
  CAN_message_t msg2;
  CAN_message_t msg3;

  CAN_message_t frame;
  CAN_message_t modifiedframe;

  CAN_message_t fromGateway_frame;
  CAN_message_t modifiedGateway_Frame;

  CAN_message_t fromABS_frame;
  CAN_message_t modifiedABS_Frame1;
  CAN_message_t modifiedABS_Frame2;
  CAN_message_t modifiedABS_FramePreCS;
  CAN_message_t modifiedABS_FrameCS;


  CAN_message_t Susp_CAN_msg0xB2; // Wheel Speed Sensors
  CAN_message_t Susp_CAN_msg0x106; // Brake Pressure
  CAN_message_t Susp_CAN_msg0x1AB; // Brake Pedal Switch
  CAN_message_t Susp_CAN_msg0x391; // AccelPedal



 int OBD_Brake_Pressure = 0;
 int OBD_Cruise_Stalk_Status = 0;

#pragma endregion

#pragma region // Declare Native PT_CAN MQB Variables used for Executable CAN Functions - PT_CAN - Operational and Driver Inputs
  
  word IgnitionStatusTimerCAN;
  elapsedMillis IgnitionStatusTimer_TimeSinceLastMessage;
  elapsedMillis IgnitionStatusTimer_TimeSinceIgnitionON;


  unsigned int DS1CurrentMapSlot_0x7EE; 
  unsigned int PowerGauge_Converted;
  unsigned int TorqueGauge_Converted;

  unsigned int HazardLightsButton;

  unsigned int BrakePedalSwitchStatus;
  unsigned int BrakePedalSwitchStatus_FromGateway;
  unsigned int BrakePedalSwitchStatus_FromABS;

  unsigned int BrakePedalSwitchStatus_OverrideButtonStatus;
  unsigned int BrakePedalSwitchStatus_atTimeof_LCBumpInActivation;

  word ParkBrakeStatus_0x104_FromGatewayCAN;
  word ParkBrakeStatus_0x104_FromABSCAN;

  unsigned int ParkBrakeStatus_0x104_FromGateway;
  unsigned int ParkBrakeStatus_0x104_FromABS;

  word BrakeSwitch_0x1AB_CAN;
  word BrakeSwitch_0x1AB_CAN_FromGateway;
  word BrakeSwitch_0x1AB_CAN_FromABS;

  word BrakePressureBAR_0x106_CAN;
  word BrakePressureBAR_0x106_FromGatewayCAN;
  word BrakePressureBAR_0x106_FromABSCAN;
  word BrakePressureBAR_0x106_Extracted;
  word BrakePressureBAR_0x106_FromGateway_Extracted;
  word BrakePressureBAR_0x106_FromABS_Extracted;

  unsigned int BrakePressureBAR_0x106_Raw;
  unsigned int BrakePressureBAR_0x106_FromGateway_Raw;
  unsigned int BrakePressureBAR_0x106_FromABS_Raw;
  unsigned int BrakePressureBAR_Int;
  unsigned int BrakePressureMPA_Int;
  unsigned int BrakePressureBAR_FromGateway_Int;
  unsigned int BrakePressureBAR_FromABS_Int;
  float BrakePressureBAR_Float;
  float BrakePressureBAR_FromGateway_Float;
  float BrakePressureBAR_FromABS_Float;
  float BrakePressureMPA_FromABS_Float;

  float brakePressure;
  float BrakePressureBAR_atTimeof_LCBumpInActivation_Float;
  float BrakePressureMPA_atTimeof_LCBumpInActivation_Float;
  unsigned int ESP_0x106_BrakeStatuses_atTimeof_LCBumpInActivation_Int; // this is multiplied by 10 when needed
  unsigned int BrakePressureBAR_atTimeof_LCBumpInActivation_Int;

  word BrakePressureBAR_atTimeof_LCBumpInActivation_Raw_buf2;
  word BrakePressureBAR_atTimeof_LCBumpInActivation_Raw_buf3;

  char BrakePressureBAR_atTimeof_LCBumpInActivation_char_buf2;
  char BrakePressureBAR_atTimeof_LCBumpInActivation_char_buf3;

  unsigned int BrakePressureBAR_atTimeof_LCBumpInActivation_Int_buf0;
  unsigned int BrakePressureBAR_atTimeof_LCBumpInActivation_Int_buf1;
  unsigned int BrakePressureBAR_atTimeof_LCBumpInActivation_Int_buf2;
  unsigned int BrakePressureBAR_atTimeof_LCBumpInActivation_Int_buf3;
  unsigned int BrakePressureBAR_atTimeof_LCBumpInActivation_Int_buf4;
  unsigned int BrakePressureBAR_atTimeof_LCBumpInActivation_Int_buf5;
  unsigned int BrakePressureBAR_atTimeof_LCBumpInActivation_Int_buf6;
  unsigned int BrakePressureBAR_atTimeof_LCBumpInActivation_Int_buf7;

  uint8_t ESP_0x101_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf0;
  uint8_t ESP_0x101_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf1;
  uint8_t ESP_0x101_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf2;
  uint8_t ESP_0x101_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf3;
  uint8_t ESP_0x101_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf4;
  uint8_t ESP_0x101_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf5;
  uint8_t ESP_0x101_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf6;
  uint8_t ESP_0x101_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf7;

  uint8_t ESP_0x104_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf0;
  uint8_t ESP_0x104_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf1;
  uint8_t ESP_0x104_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf2;
  uint8_t ESP_0x104_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf3;
  uint8_t ESP_0x104_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf4;
  uint8_t ESP_0x104_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf5;
  uint8_t ESP_0x104_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf6;
  uint8_t ESP_0x104_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf7;

  uint8_t ESP_0x106_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf0;
  uint8_t ESP_0x106_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf1;
  uint8_t ESP_0x106_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf2;
  uint8_t ESP_0x106_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf3;
  uint8_t ESP_0x106_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf4;
  uint8_t ESP_0x106_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf5;
  uint8_t ESP_0x106_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf6;
  uint8_t ESP_0x106_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf7;

  uint8_t ESP_0x116_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf0;
  uint8_t ESP_0x116_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf1;
  uint8_t ESP_0x116_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf2;
  uint8_t ESP_0x116_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf3;
  uint8_t ESP_0x116_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf4;
  uint8_t ESP_0x116_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf5;
  uint8_t ESP_0x116_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf6;
  uint8_t ESP_0x116_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf7;


  uint8_t ESP_0x11E_Statuses_atTimeof_LCBumpInActivation_Int_buf0;
  uint8_t ESP_0x11E_Statuses_atTimeof_LCBumpInActivation_Int_buf1;
  uint8_t ESP_0x11E_Statuses_atTimeof_LCBumpInActivation_Int_buf2;
  uint8_t ESP_0x11E_Statuses_atTimeof_LCBumpInActivation_Int_buf3;
  uint8_t ESP_0x11E_Statuses_atTimeof_LCBumpInActivation_Int_buf4;
  uint8_t ESP_0x11E_Statuses_atTimeof_LCBumpInActivation_Int_buf5;
  uint8_t ESP_0x11E_Statuses_atTimeof_LCBumpInActivation_Int_buf6;
  uint8_t ESP_0x11E_Statuses_atTimeof_LCBumpInActivation_Int_buf7;


  uint8_t ESP_0x1AB_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf0;
  uint8_t ESP_0x1AB_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf1;
  uint8_t ESP_0x1AB_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf2;
  uint8_t ESP_0x1AB_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf3;
  uint8_t ESP_0x1AB_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf4;
  uint8_t ESP_0x1AB_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf5;
  uint8_t ESP_0x1AB_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf6;
  uint8_t ESP_0x1AB_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf7;

  uint8_t ESP_0x392_Statuses_atTimeof_LCBumpInActivation_Int_buf0;
  uint8_t ESP_0x392_Statuses_atTimeof_LCBumpInActivation_Int_buf1;
  uint8_t ESP_0x392_Statuses_atTimeof_LCBumpInActivation_Int_buf2;
  uint8_t ESP_0x392_Statuses_atTimeof_LCBumpInActivation_Int_buf3;
  uint8_t ESP_0x392_Statuses_atTimeof_LCBumpInActivation_Int_buf4;
  uint8_t ESP_0x392_Statuses_atTimeof_LCBumpInActivation_Int_buf5;
  uint8_t ESP_0x392_Statuses_atTimeof_LCBumpInActivation_Int_buf6;
  uint8_t ESP_0x392_Statuses_atTimeof_LCBumpInActivation_Int_buf7;

  uint8_t ESP_0x31B_Statuses_atTimeof_LCBumpInActivation_Int_buf0;
  uint8_t ESP_0x31B_Statuses_atTimeof_LCBumpInActivation_Int_buf1;
  uint8_t ESP_0x31B_Statuses_atTimeof_LCBumpInActivation_Int_buf2;
  uint8_t ESP_0x31B_Statuses_atTimeof_LCBumpInActivation_Int_buf3;
  uint8_t ESP_0x31B_Statuses_atTimeof_LCBumpInActivation_Int_buf4;
  uint8_t ESP_0x31B_Statuses_atTimeof_LCBumpInActivation_Int_buf5;
  uint8_t ESP_0x31B_Statuses_atTimeof_LCBumpInActivation_Int_buf6;
  uint8_t ESP_0x31B_Statuses_atTimeof_LCBumpInActivation_Int_buf7;



  uint8_t ESP_0xFD_ChassisStatuses_atTimeof_LCBumpInActivation_Int_buf0;
  uint8_t ESP_0xFD_ChassisStatuses_atTimeof_LCBumpInActivation_Int_buf1;
  uint8_t ESP_0xFD_ChassisStatuses_atTimeof_LCBumpInActivation_Int_buf2;
  uint8_t ESP_0xFD_ChassisStatuses_atTimeof_LCBumpInActivation_Int_buf3;
  uint8_t ESP_0xFD_ChassisStatuses_atTimeof_LCBumpInActivation_Int_buf4;
  uint8_t ESP_0xFD_ChassisStatuses_atTimeof_LCBumpInActivation_Int_buf5;
  uint8_t ESP_0xFD_ChassisStatuses_atTimeof_LCBumpInActivation_Int_buf6;
  uint8_t ESP_0xFD_ChassisStatuses_atTimeof_LCBumpInActivation_Int_buf7;

  uint8_t ESP_0xB2_WheelSpeedStatuses_atTimeof_LCBumpInActivation_Int_buf0;
  uint8_t ESP_0xB2_WheelSpeedStatuses_atTimeof_LCBumpInActivation_Int_buf1;
  uint8_t ESP_0xB2_WheelSpeedStatuses_atTimeof_LCBumpInActivation_Int_buf2;
  uint8_t ESP_0xB2_WheelSpeedStatuses_atTimeof_LCBumpInActivation_Int_buf3;
  uint8_t ESP_0xB2_WheelSpeedStatuses_atTimeof_LCBumpInActivation_Int_buf4;
  uint8_t ESP_0xB2_WheelSpeedStatuses_atTimeof_LCBumpInActivation_Int_buf5;
  uint8_t ESP_0xB2_WheelSpeedStatuses_atTimeof_LCBumpInActivation_Int_buf6;
  uint8_t ESP_0xB2_WheelSpeedStatuses_atTimeof_LCBumpInActivation_Int_buf7;





  word ThrottleTPS_0x391_CAN;
  word ThrottleTPS_0x391_FromGatewayCAN;
  word ThrottleTPS_0x391_FromABSCAN;
  word ThrottleTPS_0x391_PTCAN_CAN;
  word AccelPedal_0x391_CAN;
  word AccelPedal_0x391_FromGateway_CAN;
  word AccelPedal_0x391_FromABS_CAN;
  word AccelPedal_0x391_PT_CAN_CAN;
  unsigned int AccelPedal;
  unsigned int AccelPedal_0x107;


  unsigned int AccelPedal_0x391;
  unsigned int AccelPedal_0x391_FromGateway;
  unsigned int AccelPedal_0x391_FromABS;
  unsigned int AccelPedal_0x391_PT_CAN;
  unsigned int AccelPedal_0x391_Mapped;
  unsigned int AccelPedal_0x391_FromGateway_Mapped_Int;
  unsigned int AccelPedal_0x391_FromABS_Mapped_Int;
  unsigned int AccelPedal_0x391_PT_CAN_Mapped_Int;

  float AccelPedal_0x391_Float;
  float AccelPedal_0x391_PT_CAN_Float;
  float AccelPedal_0x391_Mapped_Float;
  float AccelPedal_0x391_FromGateway_Mapped_Float;
  float AccelPedal_0x391_FromABS_Mapped_Float;
  float AccelPedal_0x391_PT_CAN_Mapped_Float;

  unsigned int ThrottleTPS_0x391;
  unsigned int ThrottleTPS_0x391_FromGateway;
  unsigned int ThrottleTPS_0x391_FromABS;
  unsigned int ThrottleTPS_0x391_PT_CAN;
  unsigned int ThrottleTPS_0x391_Mapped;
  unsigned int ThrottleTPS_0x391_FromGateway_Mapped_Int;
  unsigned int ThrottleTPS_0x391_FromABS_Mapped_Int;
  unsigned int ThrottleTPS_0x391_PT_CAN_Mapped_Int;

  float ThrottleTPS_0x391_FromGateway_Float;
  float ThrottleTPS_0x391_FromABS_Float;
  float ThrottleTPS_0x391_PT_CAN_Float;
  float ThrottleTPS_0x391_Mapped_Float;
  float ThrottleTPS_0x391_FromGateway_Mapped_Float;
  float ThrottleTPS_0x391_FromABS_Mapped_Float;
  float ThrottleTPS_0x391_PT_CAN_Mapped_Float;
  // unsigned int ThrottleTPS_0x391_FromGateway_Mapped_Int;




  word MQB_TachEngineRPM_Sniffed_0x107_CAN;
  unsigned int MQB_TachEngineRPM_0x107;
  unsigned int MQB_TachEngineRPM_Sniffed_0x107_Raw;
  unsigned int MQB_TachEngineRPM_Sniffed_0x107_Int;
  unsigned int MQB_TachEngineRPM_Override_0x107_Raw;
  unsigned int MQB_TachEngineRPM_Override_0x107_Int;
  unsigned int DisplayTachometerOverride_View_Full;
  unsigned int MQB_TachEngineRPM_Override_View_8bit;


  unsigned int GearLeverPosition_Park;
  unsigned int GearLeverPosition_Reverse;
  unsigned int GearLeverPosition_Neutral;
  unsigned int GearLeverPosition_Drive;
  unsigned int GearLeverPosition_TIP;
  unsigned int GearLeverPosition_TIP_FromGateway;

  word DQ500_GoalGear0xAD_CAN;
  unsigned int DQ500_GoalGear0xAD;

  word DQ500_ClutchLockStatus_CAN;
  unsigned int DQ500_ClutchLockStatus;


  unsigned int cruiseStalkStaticPosition_0x120;
  unsigned int CruiseStalk_OFF;
  unsigned int CruiseStalk_ON;
  unsigned int CruiseStalk_UP;
  unsigned int CruiseStalk_DOWN;
  unsigned int CruiseStalk_RES;
  unsigned int CruiseStalk_FORWARD;
  unsigned int CruiseStalk_SET;
  unsigned int CruiseStalk_DistanceUP;
  unsigned int CruiseStalk_DistanceDOWN;

  unsigned int cruiseStalkStaticPosition_0x120_PT_CAN;
  unsigned int cruiseStalkStaticPosition_0x120_FromGateway;
  unsigned int CruiseStalk_OFF_FromGateway;
  unsigned int CruiseStalk_ON_FromGateway;
  unsigned int CruiseStalk_UP_FromGateway;
  unsigned int CruiseStalk_DOWN_FromGateway;
  unsigned int CruiseStalk_RES_FromGateway;
  unsigned int CruiseStalk_FORWARD_FromGateway;
  unsigned int CruiseStalk_SET_FromGateway;
  unsigned int CruiseStalk_DistanceUP_FromGateway;
  unsigned int CruiseStalk_DistanceDOWN_FromGateway;

  unsigned int cruiseStalkStaticPosition_0x120_FromABS;
  unsigned int CruiseStalk_OFF_FromABS;
  unsigned int CruiseStalk_ON_FromABS;
  unsigned int CruiseStalk_UP_FromABS;
  unsigned int CruiseStalk_DOWN_FromABS;
  unsigned int CruiseStalk_RES_FromABS;
  unsigned int CruiseStalk_FORWARD_FromABS;
  unsigned int CruiseStalk_SET_FromABS;
  unsigned int CruiseStalk_DistanceUP_FromABS;
  unsigned int CruiseStalk_DistanceDOWN_FromABS;

  unsigned int CruiseStalk_OFF_FromPT_CAN;
  unsigned int CruiseStalk_ON_FromPT_CAN;


  word cruiseStalkCAN;
  word cruiseStalk_byte1CAN;
  word cruiseStalkCAN_FromGateway;
  word cruiseStalk_byte1CAN_FromGateway;
  word cruiseStalkStaticPosition_0x120_FromABSCAN;
  word cruiseStalkStaticPosition_0x120_FromGatewayCAN;
  word cruiseStalkStaticPosition_0x120_PTCAN_CAN;


  unsigned int cruiseStalkStaticPosition_0x120_bit;
  unsigned int cruiseStalkStaticPosition_0x120_bit_FromGateway;
  unsigned int cruiseStalkStaticPosition_0x120_bit_FromABS;

  // unsigned int CruiseStalk_OFF_bit;
  unsigned int CruiseStalk_ON_bit;
  unsigned int CruiseStalk_UP_bit;
  unsigned int CruiseStalk_DOWN_bit;
  unsigned int CruiseStalk_RES_bit;
  unsigned int CruiseStalk_FORWARD_bit;
  unsigned int CruiseStalk_SET_bit;
  unsigned int CruiseStalk_DistanceUP_bit;
  unsigned int CruiseStalk_DistanceDOWN_bit;

  // unsigned int cruiseStalkStaticPosition_0x120_bit_FromGateway;
  // unsigned int CruiseStalk_OFF_bit_FromGateway;
  unsigned int CruiseStalk_ON_bit_FromGateway;
  unsigned int CruiseStalk_UP_bit_FromGateway;
  unsigned int CruiseStalk_DOWN_bit_FromGateway;
  unsigned int CruiseStalk_RES_bit_FromGateway;
  unsigned int CruiseStalk_FORWARD_bit_FromGateway;
  unsigned int CruiseStalk_SET_bit_FromGateway;
  unsigned int CruiseStalk_DistanceUP_bit_FromGateway;
  unsigned int CruiseStalk_DistanceDOWN_bit_FromGateway;



  unsigned int ConfigurationUpdateMode1;
  unsigned int ConfigurationUpdateMode1_Active;

  unsigned int ConfigurationUpdateMode_HaldexDirect;
  unsigned int ConfigurationUpdateMode_HaldexDirect_Active;

  unsigned int ParkBrakeStatus_0x104;
  unsigned int ParkBrakeStatus;

  unsigned int Paddle_UP_Pulled;
  unsigned int Paddle_DOWN_Pulled;
  unsigned int BOTH_Paddles_Pulled; 


  unsigned int mapswitch;
  unsigned int mapswitch2;
  unsigned int cruiseStalk;
  unsigned int cruiseStalk_FromGateway;
  unsigned int cruiseStalk_byte1;
  unsigned int cruiseStalk_byte2;
  unsigned int cruiseStalk_byte1_FromGateway;
  unsigned int cruiseStalk_byte2_FromGateway;

  unsigned int WGDCPassthrough;
  unsigned int TQIntvStatus;

  unsigned int PaddlePositions;
  unsigned int BrakeSwitch_0x1AB;
  unsigned int BrakeSwitch_0x1AB_FromGateway;
  unsigned int BrakeSwitch_0x1AB_FromABS;
  unsigned int HazardLightsButton_0x366;
  unsigned int GearLeverPosition_0xAD;
  unsigned int GoalGear_0xAD;
  unsigned int EngineRPM_0xA8;
  unsigned int EngineRPM_0x783;
  unsigned int EngineRPM_0x788;
  unsigned int EngineRPM_DOT_0x783;
  unsigned int EngineRPM_DOT_0x788;
  
  unsigned int temp_EngRPM;
  unsigned int EngineRPM_IdleStatus;
  unsigned int EngineRPM_ON;
  unsigned int EngineRPM_OFF;




  word WhlSpdFR_fromABS_Raw_CAN;
  word WhlSpdFL_fromABS_Raw_CAN;
  word WhlSpdRL_fromABS_Raw_CAN;
  word WhlSpdRR_fromABS_Raw_CAN;
  word WhlSpdAvg_fromABS_Float_Raw_CAN;

  float WhlSpdFR_fromABS_Float_Raw;
  float WhlSpdFL_fromABS_Float_Raw;
  float WhlSpdRL_fromABS_Float_Raw;
  float WhlSpdRR_fromABS_Float_Raw;
  float WhlSpdAvg_fromABS_Float_Raw;
  float TrueWhlSpdRR_fromABS_Float_Raw;

  unsigned int WhlSpdFR_fromABS_Int;
  unsigned int WhlSpdFL_fromABS_Int;
  unsigned int WhlSpdRL_fromABS_Int;
  unsigned int WhlSpdRR_fromABS_Int;
  unsigned int WhlSpdAvg_fromABS_Int;



  int VehSpeed;
  int WhlSpdFR;
  int WhlSpdFL;
  int WhlSpdRL;
  int WhlSpdRR;
  int WhlSpdAvg_Int;


  word WhlSpdFL_Raw_CAN;
  word WhlSpdFR_Raw_CAN;
  word WhlSpdRL_Raw_CAN;
  word WhlSpdRR_Raw_CAN;


  int WhlSpdFL_Raw;
  int WhlSpdFR_Raw;
  int WhlSpdRL_Raw;
  int WhlSpdRR_Raw;
  int WhlSpdAvg_Raw_Int;

  float VehSpeed_Float;
  float WhlSpdFR_Float;
  float WhlSpdFL_Float;
  float WhlSpdRL_Float;
  float WhlSpdRR_Float;
  float WhlSpdAvg_Float;

  float WhlSpdFR_Float_Raw;
  float WhlSpdFL_Float_Raw;
  float WhlSpdRL_Float_Raw;
  float WhlSpdRR_Float_Raw;
  float WhlSpdAvg_Float_Raw;

  unsigned char WhlSpdFR_Char;
  unsigned char WhlSpdFL_Char;
  unsigned char WhlSpdRL_Char;
  unsigned char WhlSpdRR_Char;
  unsigned char WhlSpdAvg_Char;



  float WhlSpd_FrontAxleAvg_Float;
  float WhlSpd_RearAxleAvg_Float;
  float WhlSpd_All_Avg_Float;

  float WhlSpd_FrontAxleSlipKmh_Float;
  float WhlSpd_RearAxleSlipKmh_Float;
  float WhlSpd_CenterSlipKmh_Float;

  float WhlSpd_FrontAxleSlipPct_Float;
  float WhlSpd_RearAxleSlipPct_Float;
  float WhlSpd_CenterSlipPct_Float;


  unsigned int DriveSelect_EngTrans_Mode_Raw;
  unsigned int DriveSelect_Suspension_Mode_Raw;
  unsigned int DriveSelect_Quattro_Mode_Raw;
  unsigned int DriveSelect_Steering_Mode_Raw;
  unsigned int DriveSelect_EngineSound_Mode_Raw;
  unsigned int DriveSelect_ACC_Mode_Raw; 

  unsigned int DriveSelect_EngTrans_Mode_Int;
  unsigned int DriveSelect_Suspension_Mode_Int;
  unsigned int DriveSelect_Quattro_Mode_Int;
  unsigned int DriveSelect_Steering_Mode_Int;
  unsigned int DriveSelect_EngineSound_Mode_Int;
  unsigned int DriveSelect_ACC_Mode_Int; 


  unsigned int DriveSelect_EngTrans_Mode_Comfort;
  unsigned int DriveSelect_EngTrans_Mode_Auto;
  unsigned int DriveSelect_EngTrans_Mode_Dynamic;

  unsigned int DriveSelect_Suspension_Mode_Comfort;
  unsigned int DriveSelect_Suspension_Mode_Auto;
  unsigned int DriveSelect_Suspension_Mode_Dynamic;

  unsigned int DriveSelect_Quattro_Mode_Comfort;
  unsigned int DriveSelect_Quattro_Mode_Auto;
  unsigned int DriveSelect_Quattro_Mode_Dynamic;

  unsigned int DriveSelect_Steering_Mode_Comfort;
  unsigned int DriveSelect_Steering_Mode_Auto;
  unsigned int DriveSelect_Steering_Mode_Dynamic;

  unsigned int DriveSelect_EngineSound_Mode_Comfort;
  unsigned int DriveSelect_EngineSound_Mode_Auto;
  unsigned int DriveSelect_EngineSound_Mode_Dynamic;

  unsigned int DriveSelect_ACC_Mode_Comfort;
  unsigned int DriveSelect_ACC_Mode_Auto;
  unsigned int DriveSelect_ACC_Mode_Dynamic;


  unsigned int DQ500_TargetGear_0x394;
  unsigned int DQ500_TargetGear2_0x394;
  unsigned int DQ500_CurrentGear_0x394;
  word DQ500_TargetGear_0x394_CAN;
  word DQ500_TargetGear2_0x394_CAN;
  word DQ500_CurrentGear_0x394_CAN;
  unsigned int DQ500_CurrentGear_0x7EE;

  // unsigned int DisplayGaugePVal;
  // unsigned int DisplayGaugeTVal;


  unsigned char lsbMQB_TachEngineRPM_Override_0x107, msbMQB_TachEngineRPM_Override_0x107;


  // unsigned char lsbDisplayGaugePVal, msbDisplayGaugePVal;
  // unsigned char lsbDisplayGaugeTVal, msbDisplayGaugeTVal;

  unsigned char lsbIgnitionStatus, lsbIgnitionAwake;



unsigned long TimeSinceIgnitionOff;
 word IgnitionStatusCAN;
unsigned int IgnitionStatus;
unsigned int IgnitionAwake;
unsigned int IgnitionStatusTimer;


word Susp_CAN_msg0xB2_buf0_Raw, Susp_CAN_msg0xB2_buf1_Raw, Susp_CAN_msg0xB2_buf2_Raw, Susp_CAN_msg0xB2_buf3_Raw, Susp_CAN_msg0xB2_buf4_Raw, Susp_CAN_msg0xB2_buf5_Raw, Susp_CAN_msg0xB2_buf6_Raw, Susp_CAN_msg0xB2_buf7_Raw;
word Susp_CAN_msg0x106_buf0_Raw, Susp_CAN_msg0x106_buf1_Raw, Susp_CAN_msg0x106_buf2_Raw, Susp_CAN_msg0x106_buf3_Raw, Susp_CAN_msg0x106_buf4_Raw, Susp_CAN_msg0x106_buf5_Raw, Susp_CAN_msg0x106_buf6_Raw, Susp_CAN_msg0x106_buf7_Raw;
word Susp_CAN_msg0x1AB_buf0_Raw, Susp_CAN_msg0x1AB_buf1_Raw, Susp_CAN_msg0x1AB_buf2_Raw, Susp_CAN_msg0x1AB_buf3_Raw, Susp_CAN_msg0x1AB_buf4_Raw, Susp_CAN_msg0x1AB_buf5_Raw, Susp_CAN_msg0x1AB_buf6_Raw, Susp_CAN_msg0x1AB_buf7_Raw;
word Susp_CAN_msg0x391_buf0_Raw, Susp_CAN_msg0x391_buf1_Raw, Susp_CAN_msg0x391_buf2_Raw, Susp_CAN_msg0x391_buf3_Raw, Susp_CAN_msg0x391_buf4_Raw, Susp_CAN_msg0x391_buf5_Raw, Susp_CAN_msg0x391_buf6_Raw, Susp_CAN_msg0x391_buf7_Raw;
word Susp_CAN_msg0x394_buf0_Raw, Susp_CAN_msg0x394_buf1_Raw, Susp_CAN_msg0x394_buf2_Raw, Susp_CAN_msg0x394_buf3_Raw, Susp_CAN_msg0x394_buf4_Raw, Susp_CAN_msg0x394_buf5_Raw, Susp_CAN_msg0x394_buf6_Raw, Susp_CAN_msg0x394_buf7_Raw;
word Susp_CAN_msg0x12B_buf0_Raw, Susp_CAN_msg0x12B_buf1_Raw, Susp_CAN_msg0x12B_buf2_Raw, Susp_CAN_msg0x12B_buf3_Raw, Susp_CAN_msg0x12B_buf4_Raw, Susp_CAN_msg0x12B_buf5_Raw, Susp_CAN_msg0x12B_buf6_Raw, Susp_CAN_msg0x12B_buf7_Raw;
word Susp_CAN_msg0x86_buf0_Raw, Susp_CAN_msg0x86_buf1_Raw, Susp_CAN_msg0x86_buf2_Raw, Susp_CAN_msg0x86_buf3_Raw, Susp_CAN_msg0x86_buf4_Raw, Susp_CAN_msg0x86_buf5_Raw, Susp_CAN_msg0x86_buf6_Raw, Susp_CAN_msg0x86_buf7_Raw;
word Susp_CAN_msg0x104_buf0_Raw, Susp_CAN_msg0x104_buf1_Raw, Susp_CAN_msg0x104_buf2_Raw, Susp_CAN_msg0x104_buf3_Raw, Susp_CAN_msg0x104_buf4_Raw, Susp_CAN_msg0x104_buf5_Raw, Susp_CAN_msg0x104_buf6_Raw, Susp_CAN_msg0x104_buf7_Raw;
word Susp_CAN_msg0xA8_buf0_Raw, Susp_CAN_msg0xA8_buf1_Raw, Susp_CAN_msg0xA8_buf2_Raw, Susp_CAN_msg0xA8_buf3_Raw, Susp_CAN_msg0xA8_buf4_Raw, Susp_CAN_msg0xA8_buf5_Raw, Susp_CAN_msg0xA8_buf6_Raw, Susp_CAN_msg0xA8_buf7_Raw;


word Susp_CAN_msg0xB2_FromABS_buf0_Raw, Susp_CAN_msg0xB2_FromABS_buf1_Raw, Susp_CAN_msg0xB2_FromABS_buf2_Raw, Susp_CAN_msg0xB2_FromABS_buf3_Raw, Susp_CAN_msg0xB2_FromABS_buf4_Raw, Susp_CAN_msg0xB2_FromABS_buf5_Raw, Susp_CAN_msg0xB2_FromABS_buf6_Raw, Susp_CAN_msg0xB2_FromABS_buf7_Raw;
word Susp_CAN_msg0x104_FromABS_buf0_Raw, Susp_CAN_msg0x104_FromABS_buf1_Raw, Susp_CAN_msg0x104_FromABS_buf2_Raw, Susp_CAN_msg0x104_FromABS_buf3_Raw, Susp_CAN_msg0x104_FromABS_buf4_Raw, Susp_CAN_msg0x104_FromABS_buf5_Raw, Susp_CAN_msg0x104_FromABS_buf6_Raw, Susp_CAN_msg0x104_FromABS_buf7_Raw;
word Susp_CAN_msg0x106_FromABS_buf0_Raw, Susp_CAN_msg0x106_FromABS_buf1_Raw, Susp_CAN_msg0x106_FromABS_buf2_Raw, Susp_CAN_msg0x106_FromABS_buf3_Raw, Susp_CAN_msg0x106_FromABS_buf4_Raw, Susp_CAN_msg0x106_FromABS_buf5_Raw, Susp_CAN_msg0x106_FromABS_buf6_Raw, Susp_CAN_msg0x106_FromABS_buf7_Raw;
word Susp_CAN_msg0x120_FromABS_buf0_Raw, Susp_CAN_msg0x120_FromABS_buf1_Raw, Susp_CAN_msg0x120_FromABS_buf2_Raw, Susp_CAN_msg0x120_FromABS_buf3_Raw, Susp_CAN_msg0x120_FromABS_buf4_Raw, Susp_CAN_msg0x120_FromABS_buf5_Raw, Susp_CAN_msg0x120_FromABS_buf6_Raw, Susp_CAN_msg0x120_FromABS_buf7_Raw;
word Susp_CAN_msg0x12B_FromABS_buf0_Raw, Susp_CAN_msg0x12B_FromABS_buf1_Raw, Susp_CAN_msg0x12B_FromABS_buf2_Raw, Susp_CAN_msg0x12B_FromABS_buf3_Raw, Susp_CAN_msg0x12B_FromABS_buf4_Raw, Susp_CAN_msg0x12B_FromABS_buf5_Raw, Susp_CAN_msg0x12B_FromABS_buf6_Raw, Susp_CAN_msg0x12B_FromABS_buf7_Raw;
word Susp_CAN_msg0x1AB_FromABS_buf0_Raw, Susp_CAN_msg0x1AB_FromABS_buf1_Raw, Susp_CAN_msg0x1AB_FromABS_buf2_Raw, Susp_CAN_msg0x1AB_FromABS_buf3_Raw, Susp_CAN_msg0x1AB_FromABS_buf4_Raw, Susp_CAN_msg0x1AB_FromABS_buf5_Raw, Susp_CAN_msg0x1AB_FromABS_buf6_Raw, Susp_CAN_msg0x1AB_FromABS_buf7_Raw;
word Susp_CAN_msg0x391_FromABS_buf0_Raw, Susp_CAN_msg0x391_FromABS_buf1_Raw, Susp_CAN_msg0x391_FromABS_buf2_Raw, Susp_CAN_msg0x391_FromABS_buf3_Raw, Susp_CAN_msg0x391_FromABS_buf4_Raw, Susp_CAN_msg0x391_FromABS_buf5_Raw, Susp_CAN_msg0x391_FromABS_buf6_Raw, Susp_CAN_msg0x391_FromABS_buf7_Raw;
word Susp_CAN_msg0x394_FromABS_buf0_Raw, Susp_CAN_msg0x394_FromABS_buf1_Raw, Susp_CAN_msg0x394_FromABS_buf2_Raw, Susp_CAN_msg0x394_FromABS_buf3_Raw, Susp_CAN_msg0x394_FromABS_buf4_Raw, Susp_CAN_msg0x394_FromABS_buf5_Raw, Susp_CAN_msg0x394_FromABS_buf6_Raw, Susp_CAN_msg0x394_FromABS_buf7_Raw;

word Susp_CAN_msg0x104_FromGateway_buf0_Raw, Susp_CAN_msg0x104_FromGateway_buf1_Raw, Susp_CAN_msg0x104_FromGateway_buf2_Raw, Susp_CAN_msg0x104_FromGateway_buf3_Raw, Susp_CAN_msg0x104_FromGateway_buf4_Raw, Susp_CAN_msg0x104_FromGateway_buf5_Raw, Susp_CAN_msg0x104_FromGateway_buf6_Raw, Susp_CAN_msg0x104_FromGateway_buf7_Raw;
word Susp_CAN_msg0x106_FromGateway_buf0_Raw, Susp_CAN_msg0x106_FromGateway_buf1_Raw, Susp_CAN_msg0x106_FromGateway_buf2_Raw, Susp_CAN_msg0x106_FromGateway_buf3_Raw, Susp_CAN_msg0x106_FromGateway_buf4_Raw, Susp_CAN_msg0x106_FromGateway_buf5_Raw, Susp_CAN_msg0x106_FromGateway_buf6_Raw, Susp_CAN_msg0x106_FromGateway_buf7_Raw;
word Susp_CAN_msg0x120_FromGateway_buf0_Raw, Susp_CAN_msg0x120_FromGateway_buf1_Raw, Susp_CAN_msg0x120_FromGateway_buf2_Raw, Susp_CAN_msg0x120_FromGateway_buf3_Raw, Susp_CAN_msg0x120_FromGateway_buf4_Raw, Susp_CAN_msg0x120_FromGateway_buf5_Raw, Susp_CAN_msg0x120_FromGateway_buf6_Raw, Susp_CAN_msg0x120_FromGateway_buf7_Raw;
word Susp_CAN_msg0x12B_FromGateway_buf0_Raw, Susp_CAN_msg0x12B_FromGateway_buf1_Raw, Susp_CAN_msg0x12B_FromGateway_buf2_Raw, Susp_CAN_msg0x12B_FromGateway_buf3_Raw, Susp_CAN_msg0x12B_FromGateway_buf4_Raw, Susp_CAN_msg0x12B_FromGateway_buf5_Raw, Susp_CAN_msg0x12B_FromGateway_buf6_Raw, Susp_CAN_msg0x12B_FromGateway_buf7_Raw;
word Susp_CAN_msg0x1AB_FromGateway_buf0_Raw, Susp_CAN_msg0x1AB_FromGateway_buf1_Raw, Susp_CAN_msg0x1AB_FromGateway_buf2_Raw, Susp_CAN_msg0x1AB_FromGateway_buf3_Raw, Susp_CAN_msg0x1AB_FromGateway_buf4_Raw, Susp_CAN_msg0x1AB_FromGateway_buf5_Raw, Susp_CAN_msg0x1AB_FromGateway_buf6_Raw, Susp_CAN_msg0x1AB_FromGateway_buf7_Raw;
word Susp_CAN_msg0x391_FromGateway_buf0_Raw, Susp_CAN_msg0x391_FromGateway_buf1_Raw, Susp_CAN_msg0x391_FromGateway_buf2_Raw, Susp_CAN_msg0x391_FromGateway_buf3_Raw, Susp_CAN_msg0x391_FromGateway_buf4_Raw, Susp_CAN_msg0x391_FromGateway_buf5_Raw, Susp_CAN_msg0x391_FromGateway_buf6_Raw, Susp_CAN_msg0x391_FromGateway_buf7_Raw;
word Susp_CAN_msg0x394_FromGateway_buf0_Raw, Susp_CAN_msg0x394_FromGateway_buf1_Raw, Susp_CAN_msg0x394_FromGateway_buf2_Raw, Susp_CAN_msg0x394_FromGateway_buf3_Raw, Susp_CAN_msg0x394_FromGateway_buf4_Raw, Susp_CAN_msg0x394_FromGateway_buf5_Raw, Susp_CAN_msg0x394_FromGateway_buf6_Raw, Susp_CAN_msg0x394_FromGateway_buf7_Raw;

word PT_CAN_msg0x40_buf0_Raw, PT_CAN_msg0x40_buf1_Raw, PT_CAN_msg0x40_buf2_Raw, PT_CAN_msg0x40_buf3_Raw, PT_CAN_msg0x40_buf4_Raw, PT_CAN_msg0x40_buf5_Raw, PT_CAN_msg0x40_buf6_Raw, PT_CAN_msg0x40_buf7_Raw;
word PT_CAN_msg0x104_buf0_Raw, PT_CAN_msg0x104_buf1_Raw, PT_CAN_msg0x104_buf2_Raw, PT_CAN_msg0x104_buf3_Raw, PT_CAN_msg0x104_buf4_Raw, PT_CAN_msg0x104_buf5_Raw, PT_CAN_msg0x104_buf6_Raw, PT_CAN_msg0x104_buf7_Raw;
word PT_CAN_msg0x106_buf0_Raw, PT_CAN_msg0x106_buf1_Raw, PT_CAN_msg0x106_buf2_Raw, PT_CAN_msg0x106_buf3_Raw, PT_CAN_msg0x106_buf4_Raw, PT_CAN_msg0x106_buf5_Raw, PT_CAN_msg0x106_buf6_Raw, PT_CAN_msg0x106_buf7_Raw;
word PT_CAN_msg0x120_buf0_Raw, PT_CAN_msg0x120_buf1_Raw, PT_CAN_msg0x120_buf2_Raw, PT_CAN_msg0x120_buf3_Raw, PT_CAN_msg0x120_buf4_Raw, PT_CAN_msg0x120_buf5_Raw, PT_CAN_msg0x120_buf6_Raw, PT_CAN_msg0x120_buf7_Raw;
word PT_CAN_msg0x12B_buf0_Raw, PT_CAN_msg0x12B_buf1_Raw, PT_CAN_msg0x12B_buf2_Raw, PT_CAN_msg0x12B_buf3_Raw, PT_CAN_msg0x12B_buf4_Raw, PT_CAN_msg0x12B_buf5_Raw, PT_CAN_msg0x12B_buf6_Raw, PT_CAN_msg0x12B_buf7_Raw;
word PT_CAN_msg0x1AB_buf0_Raw, PT_CAN_msg0x1AB_buf1_Raw, PT_CAN_msg0x1AB_buf2_Raw, PT_CAN_msg0x1AB_buf3_Raw, PT_CAN_msg0x1AB_buf4_Raw, PT_CAN_msg0x1AB_buf5_Raw, PT_CAN_msg0x1AB_buf6_Raw, PT_CAN_msg0x1AB_buf7_Raw;
word PT_CAN_msg0x31B_buf0_Raw, PT_CAN_msg0x31B_buf1_Raw, PT_CAN_msg0x31B_buf2_Raw, PT_CAN_msg0x31B_buf3_Raw, PT_CAN_msg0x31B_buf4_Raw, PT_CAN_msg0x31B_buf5_Raw, PT_CAN_msg0x31B_buf6_Raw, PT_CAN_msg0x31B_buf7_Raw;
word PT_CAN_msg0x391_buf0_Raw, PT_CAN_msg0x391_buf1_Raw, PT_CAN_msg0x391_buf2_Raw, PT_CAN_msg0x391_buf3_Raw, PT_CAN_msg0x391_buf4_Raw, PT_CAN_msg0x391_buf5_Raw, PT_CAN_msg0x391_buf6_Raw, PT_CAN_msg0x391_buf7_Raw;
word PT_CAN_msg0x3DD_buf0_Raw, PT_CAN_msg0x3DD_buf1_Raw, PT_CAN_msg0x3DD_buf2_Raw, PT_CAN_msg0x3DD_buf3_Raw, PT_CAN_msg0x3DD_buf4_Raw, PT_CAN_msg0x3DD_buf5_Raw, PT_CAN_msg0x3DD_buf6_Raw, PT_CAN_msg0x3DD_buf7_Raw;
word PT_CAN_msg0x394_buf0_Raw, PT_CAN_msg0x394_buf1_Raw, PT_CAN_msg0x394_buf2_Raw, PT_CAN_msg0x394_buf3_Raw, PT_CAN_msg0x394_buf4_Raw, PT_CAN_msg0x394_buf5_Raw, PT_CAN_msg0x394_buf6_Raw, PT_CAN_msg0x394_buf7_Raw;




unsigned int Susp_CAN_msg0xB2_buf0, Susp_CAN_msg0xB2_buf1, Susp_CAN_msg0xB2_buf2, Susp_CAN_msg0xB2_buf3, Susp_CAN_msg0xB2_buf4, Susp_CAN_msg0xB2_buf5, Susp_CAN_msg0xB2_buf6, Susp_CAN_msg0xB2_buf7;
unsigned int Susp_CAN_msg0x106_buf0, Susp_CAN_msg0x106_buf1, Susp_CAN_msg0x106_buf2, Susp_CAN_msg0x106_buf3, Susp_CAN_msg0x106_buf4, Susp_CAN_msg0x106_buf5, Susp_CAN_msg0x106_buf6, Susp_CAN_msg0x106_buf7;
unsigned int Susp_CAN_msg0x1AB_buf0, Susp_CAN_msg0x1AB_buf1, Susp_CAN_msg0x1AB_buf2, Susp_CAN_msg0x1AB_buf3, Susp_CAN_msg0x1AB_buf4, Susp_CAN_msg0x1AB_buf5, Susp_CAN_msg0x1AB_buf6, Susp_CAN_msg0x1AB_buf7;
unsigned int Susp_CAN_msg0x391_buf0, Susp_CAN_msg0x391_buf1, Susp_CAN_msg0x391_buf2, Susp_CAN_msg0x391_buf3, Susp_CAN_msg0x391_buf4, Susp_CAN_msg0x391_buf5, Susp_CAN_msg0x391_buf6, Susp_CAN_msg0x391_buf7;


unsigned int Susp_CAN_msg0xB2_FromABS_buf0, Susp_CAN_msg0xB2_FromABS_buf1, Susp_CAN_msg0xB2_FromABS_buf2, Susp_CAN_msg0xB2_FromABS_buf3, Susp_CAN_msg0xB2_FromABS_buf4, Susp_CAN_msg0xB2_FromABS_buf5, Susp_CAN_msg0xB2_FromABS_buf6, Susp_CAN_msg0xB2_FromABS_buf7;
unsigned int Susp_CAN_msg0x104_FromABS_buf0, Susp_CAN_msg0x104_FromABS_buf1, Susp_CAN_msg0x104_FromABS_buf2, Susp_CAN_msg0x104_FromABS_buf3, Susp_CAN_msg0x104_FromABS_buf4, Susp_CAN_msg0x104_FromABS_buf5, Susp_CAN_msg0x104_FromABS_buf6, Susp_CAN_msg0x104_FromABS_buf7;
unsigned int Susp_CAN_msg0x106_FromABS_buf0, Susp_CAN_msg0x106_FromABS_buf1, Susp_CAN_msg0x106_FromABS_buf2, Susp_CAN_msg0x106_FromABS_buf3, Susp_CAN_msg0x106_FromABS_buf4, Susp_CAN_msg0x106_FromABS_buf5, Susp_CAN_msg0x106_FromABS_buf6, Susp_CAN_msg0x106_FromABS_buf7;
unsigned int Susp_CAN_msg0x120_FromABS_buf0, Susp_CAN_msg0x120_FromABS_buf1, Susp_CAN_msg0x120_FromABS_buf2, Susp_CAN_msg0x120_FromABS_buf3, Susp_CAN_msg0x120_FromABS_buf4, Susp_CAN_msg0x120_FromABS_buf5, Susp_CAN_msg0x120_FromABS_buf6, Susp_CAN_msg0x120_FromABS_buf7;
unsigned int Susp_CAN_msg0x12B_FromABS_buf0, Susp_CAN_msg0x12B_FromABS_buf1, Susp_CAN_msg0x12B_FromABS_buf2, Susp_CAN_msg0x12B_FromABS_buf3, Susp_CAN_msg0x12B_FromABS_buf4, Susp_CAN_msg0x12B_FromABS_buf5, Susp_CAN_msg0x12B_FromABS_buf6, Susp_CAN_msg0x12B_FromABS_buf7;
unsigned int Susp_CAN_msg0x1AB_FromABS_buf0, Susp_CAN_msg0x1AB_FromABS_buf1, Susp_CAN_msg0x1AB_FromABS_buf2, Susp_CAN_msg0x1AB_FromABS_buf3, Susp_CAN_msg0x1AB_FromABS_buf4, Susp_CAN_msg0x1AB_FromABS_buf5, Susp_CAN_msg0x1AB_FromABS_buf6, Susp_CAN_msg0x1AB_FromABS_buf7;
unsigned int Susp_CAN_msg0x391_FromABS_buf0, Susp_CAN_msg0x391_FromABS_buf1, Susp_CAN_msg0x391_FromABS_buf2, Susp_CAN_msg0x391_FromABS_buf3, Susp_CAN_msg0x391_FromABS_buf4, Susp_CAN_msg0x391_FromABS_buf5, Susp_CAN_msg0x391_FromABS_buf6, Susp_CAN_msg0x391_FromABS_buf7;


unsigned int Susp_CAN_msg0x104_FromGateway_buf0, Susp_CAN_msg0x104_FromGateway_buf1, Susp_CAN_msg0x104_FromGateway_buf2, Susp_CAN_msg0x104_FromGateway_buf3, Susp_CAN_msg0x104_FromGateway_buf4, Susp_CAN_msg0x104_FromGateway_buf5, Susp_CAN_msg0x104_FromGateway_buf6, Susp_CAN_msg0x104_FromGateway_buf7;
unsigned int Susp_CAN_msg0x106_FromGateway_buf0, Susp_CAN_msg0x106_FromGateway_buf1, Susp_CAN_msg0x106_FromGateway_buf2, Susp_CAN_msg0x106_FromGateway_buf3, Susp_CAN_msg0x106_FromGateway_buf4, Susp_CAN_msg0x106_FromGateway_buf5, Susp_CAN_msg0x106_FromGateway_buf6, Susp_CAN_msg0x106_FromGateway_buf7;
unsigned int Susp_CAN_msg0x120_FromGateway_buf0, Susp_CAN_msg0x120_FromGateway_buf1, Susp_CAN_msg0x120_FromGateway_buf2, Susp_CAN_msg0x120_FromGateway_buf3, Susp_CAN_msg0x120_FromGateway_buf4, Susp_CAN_msg0x120_FromGateway_buf5, Susp_CAN_msg0x120_FromGateway_buf6, Susp_CAN_msg0x120_FromGateway_buf7;
unsigned int Susp_CAN_msg0x12B_FromGateway_buf0, Susp_CAN_msg0x12B_FromGateway_buf1, Susp_CAN_msg0x12B_FromGateway_buf2, Susp_CAN_msg0x12B_FromGateway_buf3, Susp_CAN_msg0x12B_FromGateway_buf4, Susp_CAN_msg0x12B_FromGateway_buf5, Susp_CAN_msg0x12B_FromGateway_buf6, Susp_CAN_msg0x12B_FromGateway_buf7;
unsigned int Susp_CAN_msg0x1AB_FromGateway_buf0, Susp_CAN_msg0x1AB_FromGateway_buf1, Susp_CAN_msg0x1AB_FromGateway_buf2, Susp_CAN_msg0x1AB_FromGateway_buf3, Susp_CAN_msg0x1AB_FromGateway_buf4, Susp_CAN_msg0x1AB_FromGateway_buf5, Susp_CAN_msg0x1AB_FromGateway_buf6, Susp_CAN_msg0x1AB_FromGateway_buf7;
unsigned int Susp_CAN_msg0x391_FromGateway_buf0, Susp_CAN_msg0x391_FromGateway_buf1, Susp_CAN_msg0x391_FromGateway_buf2, Susp_CAN_msg0x391_FromGateway_buf3, Susp_CAN_msg0x391_FromGateway_buf4, Susp_CAN_msg0x391_FromGateway_buf5, Susp_CAN_msg0x391_FromGateway_buf6, Susp_CAN_msg0x391_FromGateway_buf7;
unsigned int Susp_CAN_msg0x394_FromGateway_buf0, Susp_CAN_msg0x394_FromGateway_buf1, Susp_CAN_msg0x394_FromGateway_buf2, Susp_CAN_msg0x394_FromGateway_buf3, Susp_CAN_msg0x394_FromGateway_buf4, Susp_CAN_msg0x394_FromGateway_buf5, Susp_CAN_msg0x394_FromGateway_buf6, Susp_CAN_msg0x394_FromGateway_buf7;


unsigned int PT_CAN_msg0x40_buf0, PT_CAN_msg0x40_buf1, PT_CAN_msg0x40_buf2, PT_CAN_msg0x40_buf3, PT_CAN_msg0x40_buf4, PT_CAN_msg0x40_buf5, PT_CAN_msg0x40_buf6, PT_CAN_msg0x40_buf7;
unsigned int PT_CAN_msg0x120_buf0, PT_CAN_msg0x120_buf1, PT_CAN_msg0x120_buf2, PT_CAN_msg0x120_buf3, PT_CAN_msg0x120_buf4, PT_CAN_msg0x120_buf5, PT_CAN_msg0x120_buf6, PT_CAN_msg0x120_buf7;
unsigned int PT_CAN_msg0x1AB_buf0, PT_CAN_msg0x1AB_buf1, PT_CAN_msg0x1AB_buf2, PT_CAN_msg0x1AB_buf3, PT_CAN_msg0x1AB_buf4, PT_CAN_msg0x1AB_buf5, PT_CAN_msg0x1AB_buf6, PT_CAN_msg0x1AB_buf7;
unsigned int PT_CAN_msg0x31B_buf0, PT_CAN_msg0x31B_buf1, PT_CAN_msg0x31B_buf2, PT_CAN_msg0x31B_buf3, PT_CAN_msg0x31B_buf4, PT_CAN_msg0x31B_buf5, PT_CAN_msg0x31B_buf6, PT_CAN_msg0x31B_buf7;
unsigned int PT_CAN_msg0x391_buf0, PT_CAN_msg0x391_buf1, PT_CAN_msg0x391_buf2, PT_CAN_msg0x391_buf3, PT_CAN_msg0x391_buf4, PT_CAN_msg0x391_buf5, PT_CAN_msg0x391_buf6, PT_CAN_msg0x391_buf7;
unsigned int PT_CAN_msg0x3DD_buf0, PT_CAN_msg0x3DD_buf1, PT_CAN_msg0x3DD_buf2, PT_CAN_msg0x3DD_buf3, PT_CAN_msg0x3DD_buf4, PT_CAN_msg0x3DD_buf5, PT_CAN_msg0x3DD_buf6, PT_CAN_msg0x3DD_buf7;


#pragma endregion

#pragma region // DBC Parsed Variables ****************************


  #pragma region // 0x101 - ESP Chassis Movement/Acceleration
    bool     MQB_ESP_0x101_QBit_Yaw_Rate;                    // bit 12
    bool     MQB_ESP_0x101_QBit_LongitudinalMovement;        // bit 13
    bool     MQB_ESP_0x101_QBit_LateralMovement;             // bit 14
    bool     MQB_ESP_0x101_Standstill_Flag;                  // bit 15
    float    MQB_ESP_0x101_LateralAcceleration;              // bits 16–23 (0.01, -1.27)
    float    MQB_ESP_0x101_LongitudinalAcceleration;         // bits 24–33 (10 bits, 0.03125, -16)
    float    MQB_ESP_0x101_Distribution_Wankmom;             // bits 34–38 (5 bits, 0.1, -1)
    bool     MQB_ESP_0x101_QBit_Initial_Value_Wank;          // bit 39
    float    MQB_ESP_0x101_Yaw_Rate;                         // bits 40–53 (14 bits, 0.01, 0)
    bool     MQB_ESP_0x101_VZ_YawRate;                       // bit 54
    bool     MQB_ESP_0x101_EmergencyBrakeIndicator;          // bit 55
    bool     MQB_ESP_0x101_Voltage_Initial;                  // bit 56
    uint8_t  MQB_ESP_0x101_PLA_Abort;                        // bits 57–59
    uint8_t  MQB_ESP_0x101_Status_ESP_PLA;                   // bits 60–63
  #pragma endregion

  #pragma region // 0x106 - ESP - Brake Pressure and Statuses
    float MQB_ESP05_0x106_BrakePressureMPA_Float;
    float MQB_ESP05_0x106_BrakePressureBAR_Float;

    uint8_t  MQB_ESP05_0x106_CHECKSUM;
    uint8_t  MQB_ESP05_0x106_COUNTER;
    bool     MQB_ESP05_0x106_QBit_Brake_Pressure;
    bool     MQB_ESP05_0x106_QBit_Driver_Brakes;
    uint8_t  MQB_ESP05_0x106_Threshold_Negative_Pressure;
    float    MQB_ESP05_0x106_Brake_Pressure;
    bool     MQB_ESP05_0x106_Driver_Brakes;
    bool     MQB_ESP05_0x106_Directory_TSK_Active;
    bool     MQB_ESP05_0x106_Steering_Intervention_ADS;
    bool     MQB_ESP05_0x106_Consistent_TSK;
    bool     MQB_ESP05_0x106_Brake_Pressure_AWV2;
    bool     MQB_ESP05_0x106_Consistent_AWV2;
    bool     MQB_ECD_0x106_Error;
    bool     MQB_ECD_0x106_Not_Available;
    bool     MQB_ESP05_0x106_Status_Brake_Temp;
    bool     MQB_ESP05_0x106_Autohold_Standby;
    bool     MQB_ESP05_0x106_HDC_Standby;
    bool     MQB_ESP05_0x106_HBA_Active;
    bool     MQB_ESP05_0x106_Prefill_Triggered;
    bool     MQB_ESP05_0x106_Reversing_Detected;
    bool     MQB_ESP05_0x106_Status_Starting_Aid;
    bool     MQB_ESP05_0x106_HDC_aktiv;
    uint8_t  MQB_ESP05_0x106_StartStopp_Info;
    bool     MQB_ESP05_0x106_Eingr_HL;
    bool     MQB_ESP05_0x106_Eingr_HR;
    bool     MQB_ESP05_0x106_Eingr_VL;
    bool     MQB_ESP05_0x106_Eingr_VR;
    uint16_t MQB_ESP05_0x106_BKV_Negative_Pressure;
    bool     MQB_ESP05_0x106_Autohold_Active;
    bool     MQB_ESP05_0x106_FStatus_Starting_Aid;
    bool     MQB_ESP05_0x106_Verz_EPB_aktiv;
    bool     MQB_ECD_0x106_Brake_Light;
    bool     MQB_ESP05_0x106_Delay_EPB_Available;
    bool     MQB_ESP05_0x106_Status_Brake_Pressure;
    uint8_t  MQB_ESP05_0x106_Requirement_EPB;

  #pragma endregion

  #pragma region // 0x11E - ESP_08 - Drivetrain - Brake

    uint8_t  MQB_ESP08_0x11E_08_CRC;                          // bits 0–7
    uint8_t  MQB_ESP08_0x11E_08_BZ;                           // bits 8–11
    bool     MQB_ESP08_0x11E_ANB_CM_Reonversion;              // bit 12
    bool     MQB_ESP08_0x11E_Consistency_ACC_Message;         // bit 13
    bool     MQB_ESP08_0x11E_StandStillPhase_Exhausted;       // bit 14
    bool     MQB_ESP08_0x11E_ZT_ReConversion;                 // bit 15
    bool     MQB_ESP08_0x11E_DoorContact_DriversDoor;         // bit 16
    bool     MQB_ESP08_0x11E_Slipping_Stillstand;             // bit 18
    bool     MQB_ESP08_0x11E_Fahrer_tritt_ZBR_Schw;           // bit 19
    bool     MQB_ESP08_0x11E_QBit_v_ref;                      // bit 41
    uint8_t  MQB_ESP08_0x11E_VehicleSpeed_ref_Direction_Of_Travel;       // bits 42–43
    uint16_t MQB_ESC_0x11E_Brake_Pressure_Gradient;         // bits 44–51 (scaled)
    float    MQB_ESP08_0x11E_VehicleSpeed_ref;                           // bits 52–63 (12 bits scaled)

  #pragma endregion

  #pragma region // 0x116 - ESP
    uint8_t  MQB_ESP10_0x116_CHECKSUM;                           // bits 0–7
    uint8_t  MQB_ESP10_0x116_COUNTER;                            // bits 8–11
    bool     MQB_ESP10_0x116_QBit_Path_Impulse_VL;           // bit 12
    bool     MQB_ESP10_0x116_QBit_Path_Impulse_VR;           // bit 13
    bool     MQB_ESP10_0x116_QBit_Path_Impulse_HL;           // bit 14
    bool     MQB_ESP10_0x116_QBit_Path_Impulse_HR;           // bit 15

    uint16_t MQB_ESP10_0x116_Path_Impulse_VL;                // bits 16–25, scale (1,0)
    uint16_t MQB_ESP10_0x116_Path_Impulse_VR;                // bits 26–35
    uint16_t MQB_ESP10_0x116_Path_Impulse_HL;                // bits 36–45
    uint16_t MQB_ESP10_0x116_Path_Impulse_HR;                // bits 46–55

    uint8_t  MQB_ESP10_0x116_VL_Direction_of_Travel;         // bits 56–57
    uint8_t  MQB_ESP10_0x116_VR_Direction_of_Travel;         // bits 58–59
    uint8_t  MQB_ESP10_0x116_HL_Direction_of_Travel;         // bits 60–61
    uint8_t  MQB_ESP10_0x116_HR_Direction_of_Travel;         // bits 62–63

  #pragma endregion

  #pragma region // 0x104 - EPB
    uint8_t  MQB_EPB_0x104_01_CRC;                         // bits 0–7
    uint8_t  MQB_EPB_0x104_01_BZ;                          // bits 8–11
    bool     MQB_EPB_0x104_QBit_Longitudinal_Acceleration; // bit 12
    bool     MQB_EPB_0x104_QBit_Clutch_Pedal_Travel;       // bit 13
    bool     MQB_EPB_0x104_BCM2_Motor_Wakeup;              // bit 14
    bool     MQB_EPB_0x104_Release_Delay_Request;          // bit 15

    float    MQB_EPB_0x104_Delay_requested;                // bits 16–23, (0.048, -7.968)
    int8_t   MQB_EPB_0x104_Longitudinal_acceleration;      // bits 24–31, (1, -128)
    float    MQB_EPB_0x104_Clutch_Pedal_Travel;            // bits 32–39, (0.4, 0)

    bool     MQB_EPB_0x104_Release_Deceleration_Start;     // bit 48
    bool     MQB_EPB_0x104_DAA_Randbed_erf;                // bit 49
    uint8_t  MQB_EPB_0x104_ErrorStatus;                    // bits 50–51
    uint8_t  MQB_EPB_0x104_Switch_Position;                // bits 52–53
    bool     MQB_EPB_0x104_QBit_Switch_Position;           // bit 54
    bool     MQB_EPB_0x104_Consistency_ACC;                 // bit 55
    uint8_t  MQB_EPB_0x104_Clamping_Force;                 // bits 56–60 (scaled 1,0)
    uint8_t  MQB_EPB_0x104_Status;                         // bits 61–62

  #pragma endregion

  #pragma region // 0x3C8 - Gearbox Status
          uint8_t  MQB_GE_0x3C8_OBD_ShutoffValve;             // bits 12–15
          float    MQB_GE_0x3C8_amax_Possible;                // bits 16–24, (0.024, -2.016)
          uint8_t  MQB_GE_0x3C8_Charisma_FahrPr;              // bits 25–28
          uint8_t  MQB_GE_0x3C8_Charisma_Status;              // bits 29–30
          uint8_t  MQB_GE_0x3C8_TorqueLoss;                // bits 32–39
          bool     MQB_GE_0x3C8_Freigabe_Verfallsinfo_WFS;    // bit 49
          bool     MQB_GE_0x3C8_Codierung_MSG;                // bit 50
          bool     MQB_GE_0x3C8_LaunchControl;                // bit 51
          uint8_t  MQB_GE_0x3C8_HeatingRequest;               // bits 52–53
          bool     MQB_GE_0x3C8_OBD_Status;                   // bit 54
          bool     MQB_GE_0x3C8_LFR_Adaption;                 // bit 55
          int16_t  MQB_GE_0x3C8_Sumpftemperatur;              // bits 56–63, (1,-58)

  #pragma endregion

  #pragma region // 0x392 - ESP/Wheel Statuses
      uint8_t  MQB_ESP07_0x392_07_CRC;                    // bits 0–7
      uint8_t  MQB_ESP07_0x392_07_BZ;                     // bits 8–11
      bool     MQB_ESP07_0x392_ACC_LDE;                   // bit 12
      bool     MQB_ESP07_0x392_Quattro_Drie;              // bit 13
      uint8_t  MQB_ESP07_0x392_Coding_ADS;                // bits 14–15

      float    MQB_ESP07_0x392_RTA_HL;                    // bits 16–23,  (0.048828125, -6.20117)
      float    MQB_ESP07_0x392_RTA_HR;                    // bits 24–31,  (0.048828125, -6.20117)
      float    MQB_ESP07_0x392_RTA_VR;                    // bits 32–39,  (0.048828125, -6.20117)

      uint8_t  MQB_OBD_0x392_Wheel_Sensor_Error_HL;     // bits 40–43
      uint8_t  MQB_OBD_0x392_Wheel_Sensor_Error_HR;     // bits 44–47
      uint8_t  MQB_OBD_0x392_Wheel_Sensor_Error_VL;     // bits 48–51
      uint8_t  MQB_OBD_0x392_Wheel_Sensor_Error_VR;     // bits 52–55

      bool     MQB_ESP07_0x392_Qualification_Drive_Type;  // bit 56
      bool     MQB_ESP07_0x392_Offroad_Mode;              // bit 57
      bool     MQB_ESP07_0x392_MKB_Triggerable;           // bit 58
      bool     MQB_ESP07_0x392_MKB_Status;                // bit 59
      bool     MQB_ESP07_0x392_CM_Variant;                // bit 60
      bool     MQB_ESP07_0x392_OBD_Status;                // bit 61

  #pragma endregion

  #pragma region // 0xFD - Chassis and system flags
    uint8_t  MQB_ESP21_0xFD_CHECKSUM;                        // bits 0–7
    uint8_t  MQB_ESP21_0xFD_COUNTER;                         // bits 8–11
    int16_t  MQB_BR_0xFD_Engagement_Torque;            // bits 12–21, (1, -509)
    bool     MQB_ESP21_0xFD_PLA_Brake_Intervention;      // bit 22
    bool     MQB_ESP21_0xFD_Diagnose;                    // bit 23
    bool     MQB_ESC_0xFD_Reku_Release;                // bit 24
    uint8_t  MQB_ESC_0xFD_v_Signal_Qualifier_High_Low; // bits 25–27
    bool     MQB_ESP21_0xFD_Feedforward_control;         // bit 28
    bool     MQB_ESP21_0xFD_AWV3_Brakes_Active;          // bit 29
    bool     MQB_OBD_0xFD_Bad_Road;                    // bit 30
    bool     MQB_OBD_0xFD_QBit_Bad_Road;               // bit 31
    float    MQB_ESP21_0xFD_VehicleSpeed_Signal;                    // bits 32–47, (0.01, 0)
    bool     MQB_ASR_0xFD_Keying_Passive;              // bit 48
    bool     MQB_ESP21_0xFD_Keying_Passive;              // bit 49
    bool     MQB_ESP21_0xFD_Systemstatus;                // bit 50
    uint8_t  MQB_ASR_0xFD_Intervention_Switch;         // bits 51–52
    bool     MQB_ESP21_0xFD_Hold_Confirmation;           // bit 53
    bool     MQB_ESP21_0xFD_MKB_Abort_Speed;             // bit 54
    bool     MQB_ESP21_0xFD_QBit_v_Signal;               // bit 55
    bool     MQB_ABS_0xFD_Braking;                     // bit 56
    bool     MQB_ASR_0xFD_Initial;                     // bit 57
    bool     MQB_MSR_0xFD_Initial;                     // bit 58
    bool     MQB_EBV_0xFD_Intervention;                // bit 59
    bool     MQB_EDS_0xFD_Intervention;                // bit 60
    bool     MQB_ESP21_0xFD_Intervention;                // bit 61
    bool     MQB_ESP21_0xFD_ASP;                         // bit 62
    bool     MQB_ESP21_0xFD_Stopping_Process_ACC_aktiv;  // bit 63

  #pragma endregion



  #pragma region // 0xB2 - Wheel Speed Sensors
    float MQB_ESP_0xB2_HL_Wheel_Speed_02;  // bits 0–15   (0.0075, 0)
    float MQB_ESP_0xB2_HR_Wheel_Speed_02;  // bits 16–31  (0.0075, 0)
    float MQB_ESP_0xB2_VL_Wheel_Speed_02;  // bits 32–47  (0.0075, 0)
    float MQB_ESP_0xB2_VR_Wheel_Speed_02;  // bits 48–63  (0.0075, 0)

  #pragma endregion

  #pragma region // 0x30B - Kombi

    // Byte 0
    bool MQB_KBI_0x30B_ABS_Lampe;
    bool MQB_KBI_0x30B_ESP_Lampe;
    bool MQB_KBI_0x30B_BKL_Lampe;
    bool MQB_KBI_0x30B_Airbag_Lampe;
    bool MQB_KBI_0x30B_SILA_gueltig;
    bool MQB_KBI_0x30B_Lenkung_Lampe;
    bool MQB_KBI_0x30B_Vorglueh_System_Lampe;
    bool MQB_KBI_0x30B_NV_in_Anzeige;

    // Byte 1
    uint8_t MQB_Kombi_01_BZ;              // bits 8–11
    bool MQB_KBI_0x30B_Anzeigestatus_ACC;       // bit 12
    uint8_t MQB_KBI_0x30B_Anzeigestatus_GRA;    // bits 13–14
    bool MQB_KBI_0x30B_OilPressure_Switch;       // bit 15

    // Byte 2
    bool MQB_KBI_0x30B_TankWarning;             // bit 16
    bool MQB_KBI_0x30B_MFA_v_Unit_01;        // bit 17
    bool MQB_KBI_0x30B_in_ActuatorTest;       // bit 18
    uint8_t MQB_KBI_0x30B_DisplayError_LDW;    // bits 19–20
    bool MQB_KBI_0x30B_Variante_USA;            // bit 21
    bool MQB_KBI_0x30B_Oeldruckwarnung;         // bit 22
    bool MQB_KBI_0x30B_HandBrake;               // bit 23

    // Bytes 3–4
    uint16_t MQB_KBI_0x30B_V_Digital;           // bits 24–32 (9 bits)
    bool MQB_KBI_0x30B_PLA_in_Anzeige;          // bit 33
    bool MQB_KBI_0x30B_DisplayError_NV;        // bit 34
    uint8_t MQB_KBI_0x30B_Anzeigestatus_LIM;    // bits 35–36

    // Bytes 6–7
    float MQB_KBI_0x30B_angez_Geschw;           // bits 48–57 (10 bits, scale 0.32 km/h)
    bool MQB_KBI_0x30B_Unit_Tacho;           // bit 58
    bool MQB_KBI_0x30B_Consistency_ACC;          // bit 59
    bool MQB_KBI_0x30B_ErrorDisplay_ACC;      // bit 60
    uint8_t MQB_KBI_0x30B_DisplayError_SWA;    // bits 61–62


  #pragma endregion

  #pragma region // 0x3BE - Motor_14


    uint8_t MQB_MO_0x3BE_StartStopp_Status;           // bits 12–13
    bool    MQB_MO_0x3BE_StartStopp_Wiederstart;      // bit 14
    bool    MQB_MO_0x3BE_StartStopp_Motorstopp;       // bit 15
    uint8_t MQB_MO_0x3BE_Freig_Reku;                  // bits 16–17
    bool    MQB_MO_0x3BE_Kl_75;                       // bit 18
    bool    MQB_MO_0x3BE_Kl_50;                       // bit 19
    uint8_t MQB_MO_0x3BE_Gangposition;                // bits 20–23
    uint8_t MQB_MO_0x3BE_StartStopp_Fahrerwunsch;     // bits 24–25
    bool    MQB_MO_0x3BE_HYB_DriverReadiness;         // bit 26
    bool    MQB_MO_0x3BE_Ext_E_Drive_Active;          // bit 27
    bool    MQB_MO_0x3BE_Driver_Brakes;               // bit 28
    bool    MQB_MO_0x3BE_QBit_Driver_Brakes;          // bit 29
    bool    MQB_MO_0x3BE_BLS;                         // bit 30
    bool    MQB_MO_0x3BE_Consistency_BrakePad;        // bit 31
    bool    MQB_MO_0x3BE_Timeout_ESP;                 // bit 32
    uint8_t MQB_MO_0x3BE_Climate_Entry;               // bits 33–34
    bool    MQB_MO_0x3BE_Special_Occasion;            // bit 35
    bool    MQB_MO_0x3BE_Release_Occasion;            // bit 36
    bool    MQB_MO_0x3BE_Clutch_Switch;               // bit 37
    bool    MQB_MO_0x3BE_Interlock;                   // bit 38
    bool    MQB_MO_0x3BE_Motor_Running;               // bit 39
    bool    MQB_MO_0x3BE_Kickdown;                    // bit 40
    bool    MQB_MO_0x3BE_Status_CylinderSwitching_01; // bit 41
    uint8_t MQB_MO_0x3BE_EKlKomLeiRed;                // bits 42–43
    bool    MQB_MO_0x3BE_Handshake_STH;               // bit 44
    bool    MQB_MO_0x3BE_BKV_LowPressureWarning;      // bit 45
    bool    MQB_MO_0x3BE_Release_Sailing;             // bit 46
    uint8_t MQB_MO_0x3BE_PTC_Status;                  // bits 47–49
    bool    MQB_MO_0x3BE_QBit_GearPosition;           // bit 50
    bool    MQB_MO_0x3BE_SignalSource_GearPosition;   // bit 51
    bool    MQB_MO_0x3BE_Remotestart_Operation;       // bit 52

    #pragma endregion

  #pragma region  // 0xAD - Gearbox_11

    uint8_t  MQB_GE_0xAD_CHECKSUM;                    // bits 0–7
    uint8_t  MQB_GE_0xAD_COUNTER_DISABLED;            // bits 8–11
    int16_t  MQB_GE_0xAD_EngineTqTarget_02;             // bits 12–21,  (1, -509)
    int16_t  MQB_GE_0xAD_EngineTqLead_02;          // bits 22–31,  (1, -509)
    float    MQB_GE_0xAD_Uefkt;                    // bits 32–41,  (0.1, 0)
    uint8_t  MQB_GE_0xAD_Fahrstufe;                // bits 42–45
    bool     MQB_GE_0xAD_reserv_Fahrstufe;         // bit 46
    uint8_t  MQB_GE_0xAD_SwitchingSequence;        // bits 47–48
    bool     MQB_GE_0xAD_Effect_Implausible;       // bit 49
    uint8_t  MQB_GE_0xAD_MMom_Status_02;           // bits 50–52
    uint8_t  MQB_GE_0xAD_Status_TractionControl;   // bits 53–55
    uint8_t  MQB_GE_0xAD_MMom_Status;              // bits 56–57
    bool     MQB_GE_0xAD_Freig_MMom_Vorhalt;       // bit 58
    bool     MQB_GE_0xAD_Prohibition_of_Hiding;    // bit 59
    uint8_t  MQB_GE_0xAD_TargetGear;               // bits 60–63


    #pragma endregion

  #pragma region  // 0x31B - ESP_24

    uint8_t  MQB_ESP24_0x31B_CHECKSUM;                // bits 0–7
    uint8_t  MQB_ESP24_0x31B_COUNTER;                 // bits 8–11
    float    MQB_ESP24_0x31B_VehicleSpeed;        // bits 16–31, (0.5, 0)
    uint16_t MQB_ESP24_0x31B_ESP24UnknownByte4;   // bits 32–47, raw 16-bit value
    uint16_t MQB_ESP24_0x31B_WheelRotationCounter;// bits 40–50, 11-bit field (rolling counter / diagnostic)

    #pragma endregion


    #pragma region  // 0x


    #pragma endregion


    #pragma region  // 0x


    #pragma endregion


    #pragma region  // 0x


    #pragma endregion


    #pragma region  // 0x


    #pragma endregion






#pragma endregion

#pragma region // Archive - not currently used


// User-supplied hook
extern void user_modify_payload(uint32_t id, uint8_t data[8], bool outbound);

static void extract_payload7(const uint8_t data[8], uint8_t out7[7]) {
  for (int i = 0; i < 7; i++) out7[i] = data[i + 1];
}

//helper
void recalcChecksum(CAN_message_t &frame) {
    uint8_t newb0 = resolve_X_from_progmem(frame.id, frame.buf);
    // frame.buf[0] = newb0;
    frame.buf[0] = computeChecksum_7B(frame.id, frame.buf);
}



#pragma endregion




 // ************************** SERIAL PRINT FUNCTION *************************
 void printFrame(CAN_message_t Pframe, int filter) {
   if (filter == 1) Serial.print("RX0: ");                 //Messages received on Can0
     else if (filter == 2 ) Serial.print("RX2_fromABS                        : ");           // 2 - Printing Raw Messages received on Can2
     else if (filter == 3 ) Serial.print("RX3_fromGateway                    : ");           // 3 - Printing Raw Messages recevied on CAN1
     else if (filter == 4 ) Serial.print("ModMsg from ABS Node to Gateway    : ");           // 4 - Modified messages from CAN2, sending to CAN1
     else if (filter == 5 ) Serial.print("ModMsg from Gateway to ABS Node    : ");           // 5 - Modified messages from CAN1, sending to Can2
     else if (filter == 6 ) Serial.print("FW_Orig from ABS_Node to Gateway   : ");           // 6 - Unmodified Messages forwarding from Can2 to CAN1
     else if (filter == 7 ) Serial.print("FW_Orig from Gateway to ABS Node   : ");           // 7 - Unmodified Messages forwarding from CAN1 to Can2
     else if (filter == 8 ) Serial.print("FW_0x106x2 from ABS_Node to Gateway: ");           // 8 - Messages forwarding from Can2 to CAN1
     //   else if (filter == 10) Serial.print("FW_0x106 from Gateway to ABS_Node  : ");          //Messages forwarding from Can0 to Can1
     //   else if (filter == 11) Serial.print("DM0: ");          //Drive Mode messages sending to Can0
     //   else if (filter == 12) Serial.print("DM2: ");          //Drive Mode messages detected on Can2
     //   else Serial.print("???");
           
           Serial.print("0x");
           Serial.print(Pframe.id, HEX); Serial.print("\t");
          //  Serial.print("\t");
          //  Serial.print(" Len: ");
          //  Serial.print(Pframe.len); Serial.print("\t");
          //  Serial.print(" Data: ");
         for (int count = 0; count < Pframe.len; count++) {
           Serial.print(Pframe.buf[count], HEX);
           Serial.print(" ");
         }
           Serial.println(" \t");
       }
 /// ---------------------------------------------------------------------------------------

void can_setup() {

pinMode(32,OUTPUT);
digitalWrite(32, HIGH);

pinMode(LED_PIN_CANRecieve, OUTPUT);


  TFTCAN1.begin();
    TFTCAN1.setRX(ALT);  // Alt pins when connecting to TFT KitchenSinkCrackHouse v2025x3 or v2025x4 -  
    TFTCAN1.setTX(ALT);

  TFTCAN1.setBaudRate(500000);
  TFTCAN1.enableFIFO();
  TFTCAN1.mailboxStatus();


  TFTCAN2.begin();
  TFTCAN2.setBaudRate(500000);
  TFTCAN2.enableFIFO();
  TFTCAN2.mailboxStatus();


  TFTCAN3.begin();
  TFTCAN3.setBaudRate(500000);
  TFTCAN3.enableFIFO();
  TFTCAN3.mailboxStatus();



// LC_BumpIn_ActivationButton = 1;

  TimeSinceIgnitionON = 0;
LC_BumpIn_TimeSinceActivation = 0;
LC_BumpIn_AcceptableConditions  = 0;
TFT_MITM_LC_BumpIn_Status_Active_Int = 0;
TFT_MITM_LC_BumpInActive_RollingStatus_Int = 0;

}


// Poll TFTCAN1 (From ABS Node), modify if it's currently in LC_BumpIn_Active
void loop_TFTCAN1_poll() {
  CAN_message_t fromABS_frame;
  CAN_message_t fromABS_frame_mod;

  // CAN2 → CAN1
  while (TFTCAN1.read(fromABS_frame)) {
    //  printFrame(fromABS_frame, 2);  // 2 - Printing Raw Messages received on Can2

    if (!id_is_watched(fromABS_frame.id)) {
          TFTCAN2.write(fromABS_frame);
    //  printFrame(fromABS_frame, 2);  // 2 - Printing Raw Messages received on Can2

    }


        else if (fromABS_frame.id == 0x101) { // ESP Chassis Movements
              //  printFrame(fromABS_frame, 2);  // 2 - Printing Raw Messages received on Can2

            #pragma region // Original Processing of 0x101 (ESP Chassis Movements) 

              ESP_02_t esp02 = parse_ESP_02(fromABS_frame);
              MQB_ESP_0x101_LongitudinalAcceleration = esp02.ESP_LongitudinalAcceleration;
              MQB_ESP_0x101_LateralAcceleration = esp02.ESP_LateralAcceleration;
              MQB_ESP_0x101_Yaw_Rate = esp02.ESP_Yaw_Rate;
              MQB_ESP_0x101_Status_ESP_PLA = esp02.ESP_Status_ESP_PLA;
              MQB_ESP_0x101_QBit_Initial_Value_Wank = esp02.ESP_QBit_Initial_Value_Wank;
              MQB_ESP_0x101_Distribution_Wankmom = esp02.ESP_Distribution_Wankmom;
              MQB_ESP_0x101_PLA_Abort = esp02.ESP_PLA_Abort;
              MQB_ESP_0x101_Standstill_Flag = esp02.ESP_Standstill_Flag;
              MQB_ESP_0x101_QBit_LongitudinalMovement = esp02.ESP_QBit_LongitudinalMovement;
              MQB_ESP_0x101_QBit_LateralMovement = esp02.ESP_LateralAcceleration;
              MQB_ESP_0x101_QBit_Yaw_Rate = esp02.ESP_Yaw_Rate;

              #pragma endregion

            #pragma region // Determine LC Bump-In Activation Status and record values at time of activation

                if(LC_BumpIn_ActivationStatus == 1 && LC_BumpIn_TimeSinceActivation < 500 && MQB_ESP05_0x106_BrakePressureMPA_Float > 3 && LC_BumpIn_0x101_CallbackRecorded == 0)  { // Trigger the start of LC BumpIn Activation , which records the initial brake pressure at time of overriding           

                        ESP_0x101_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf1 = fromABS_frame.buf[1];
                        ESP_0x101_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf2 = fromABS_frame.buf[2];
                        ESP_0x101_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf3 = fromABS_frame.buf[3];
                        ESP_0x101_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf4 = fromABS_frame.buf[4];
                        ESP_0x101_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf5 = fromABS_frame.buf[5];
                        ESP_0x101_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf6 = fromABS_frame.buf[6];
                        ESP_0x101_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf7 = fromABS_frame.buf[7];

                        LC_BumpIn_0x101_CallbackRecorded = 1;

                      }


                #pragma endregion

            #pragma region // Execute either Unmodified or Modified callback 

              if ( LC_BumpIn_ActivationStatus == 0 || LC_BumpIn_0x101_CallbackRecorded == 0 ) {  // Send unmodified frame if LC_BumpIn_ActivationStatus is NOT Active

                    TFTCAN2.write(fromABS_frame);

                    // Serial.println("Sent == 0x106 callback frame to CAN1, LC_BumpIn_ActivationStatus == 0 ");
                    // Serial.println("Sent UNmodified frame to 0x101...");

                            // printFrame(fromABS_frame, 6);  // 6 - Unmodified Messages forwarding from Can2 (ABS Node) to CAN1 (To Gateway)

                }

                else if(LC_BumpIn_ActivationStatus == 1 && LC_BumpIn_0x101_CallbackRecorded == 1 ) { // Send modified frame if LC_BumpIn_ActivationStatus IS Active

                          CAN_message_t modFrame_0x101;
                          modFrame_0x101 = fromABS_frame;

                          modFrame_0x101.buf[0] = fromABS_frame.buf[0];
                          modFrame_0x101.buf[1] = fromABS_frame.buf[1];
                          modFrame_0x101.buf[2] = 0x82;
                          modFrame_0x101.buf[3] = 0x00;
                          modFrame_0x101.buf[4] = 0x82;
                          modFrame_0x101.buf[5] = 0x05;
                          modFrame_0x101.buf[6] = 0x00;
                          modFrame_0x101.buf[7] = 0x00;  



                          bool hit = build_frame_with_checksum_0x101(modFrame_0x101.buf);

                          // if (hit) Serial.println("Checksum found in lookup");
                          // else Serial.println("Checksum fallback used");

                          // TFTCAN2.write(modFrame_0x101);
                          TFTCAN2.write(fromABS_frame);

                            // Serial.println("Sent MODIFIED frame to 0x101...");
                            // Serial.print("LC_BumpIn_0x101_CallbackRecorded: "); Serial.print(LC_BumpIn_0x101_CallbackRecorded); Serial.println("\t");                            
                           
                            // printFrame(fromABS_frame, 6);  // 6 - Unmodified Messages forwarding from Can2 (ABS Node) to CAN1 (To Gateway)
                            // printFrame(modFrame_0x101, 4); // 4 - Modified messages from CAN2, sending to CAN1

                            // // // Serial.println("---------------------------------------------------------------------------------");

                      }
                  



                #pragma endregion

          }

        else if (fromABS_frame.id == 0xFD) { // ESP21 - Chassis and system flags
          //  printFrame(fromABS_frame, 2);                                  //uncomment line to print frames received on TFTCAN2

            #pragma region // Original Processing of 0x101 (ESP Chassis Movements) 


              ESP_21_t esp21 = parse_ESP_21(fromABS_frame);
              MQB_BR_0xFD_Engagement_Torque = esp21.BR_Engagement_Torque;
              MQB_ABS_0xFD_Braking = esp21.ABS_Braking;
              MQB_ESP21_0xFD_VehicleSpeed_Signal= esp21.ESP_v_Signal;
              MQB_ESP21_0xFD_QBit_v_Signal = esp21.ESP_QBit_v_Signal;

              #pragma endregion

            #pragma region // Determine LC Bump-In Activation Status and record values at time of activation

                if(LC_BumpIn_ActivationStatus == 1 && LC_BumpIn_TimeSinceActivation < 500 && MQB_ESP05_0x106_BrakePressureMPA_Float > 3 && LC_BumpIn_0xFD_CallbackRecorded == 0)  { // Trigger the start of LC BumpIn Activation , which records the initial brake pressure at time of overriding
                // if(LC_BumpIn_ActivationStatus == 0){

                        ESP_0xFD_ChassisStatuses_atTimeof_LCBumpInActivation_Int_buf1 = fromABS_frame.buf[1];
                        ESP_0xFD_ChassisStatuses_atTimeof_LCBumpInActivation_Int_buf2 = fromABS_frame.buf[2];
                        ESP_0xFD_ChassisStatuses_atTimeof_LCBumpInActivation_Int_buf3 = fromABS_frame.buf[3];
                        ESP_0xFD_ChassisStatuses_atTimeof_LCBumpInActivation_Int_buf4 = fromABS_frame.buf[4];
                        ESP_0xFD_ChassisStatuses_atTimeof_LCBumpInActivation_Int_buf5 = fromABS_frame.buf[5];
                        ESP_0xFD_ChassisStatuses_atTimeof_LCBumpInActivation_Int_buf6 = fromABS_frame.buf[6]; 
                        ESP_0xFD_ChassisStatuses_atTimeof_LCBumpInActivation_Int_buf7 = fromABS_frame.buf[7];

                        LC_BumpIn_0xFD_CallbackRecorded = 1;

                }


                #pragma endregion

            #pragma region // Execute either Unmodified or Modified callback 

              if (LC_BumpIn_ActivationStatus == 0 || LC_BumpIn_0xFD_CallbackRecorded == 0) {  // Send unmodified frame if LC_BumpIn_ActivationStatus is NOT Active

                    TFTCAN2.write(fromABS_frame);
                    // Serial.println("Sent UNmodified frame to 0xFD...");
                    // printFrame(fromABS_frame, 6);  // 6 - Unmodified Messages forwarding from Can2 (ABS Node) to CAN1 (To Gateway)

                }

                else if(LC_BumpIn_ActivationStatus == 1 && LC_BumpIn_0xFD_CallbackRecorded == 1 ) { // Send modified frame if LC_BumpIn_ActivationStatus IS Active

                          CAN_message_t modFrame_0xFD;
                          modFrame_0xFD = fromABS_frame;

                          modFrame_0xFD.buf[0] = fromABS_frame.buf[0];
                          modFrame_0xFD.buf[1] = fromABS_frame.buf[1];
                          modFrame_0xFD.buf[2] = ESP_0xFD_ChassisStatuses_atTimeof_LCBumpInActivation_Int_buf2;
                          modFrame_0xFD.buf[3] = ESP_0xFD_ChassisStatuses_atTimeof_LCBumpInActivation_Int_buf3;
                          modFrame_0xFD.buf[4] = ESP_0xFD_ChassisStatuses_atTimeof_LCBumpInActivation_Int_buf4;
                          modFrame_0xFD.buf[5] = ESP_0xFD_ChassisStatuses_atTimeof_LCBumpInActivation_Int_buf5;
                          modFrame_0xFD.buf[6] = ESP_0xFD_ChassisStatuses_atTimeof_LCBumpInActivation_Int_buf6;
                          modFrame_0xFD.buf[7] = ESP_0xFD_ChassisStatuses_atTimeof_LCBumpInActivation_Int_buf7;



                          bool hit = build_frame_with_checksum_0xFD(modFrame_0xFD.buf);

                          // if (hit) Serial.println("Checksum found in lookup");
                          // else Serial.println("Checksum fallback used");

                          // TFTCAN2.write(modFrame_0xFD);
                          TFTCAN2.write(fromABS_frame);

                            // Serial.println("Sent MODIFIED frame to 0xFD...");
                            // Serial.print("LC_BumpIn_0xFD_CallbackRecorded: "); Serial.print(LC_BumpIn_0xFD_CallbackRecorded); Serial.println("\t");  

                            // printFrame(fromABS_frame, 6);  // 6 - Unmodified Messages forwarding from Can2 (ABS Node) to CAN1 (To Gateway)
                            // printFrame(modFrame_0xFD, 4); // 4 - Modified messages from CAN2, sending to CAN1

                            // // // Serial.println("---------------------------------------------------------------------------------");

                      }
                  




                #pragma endregion
     

        }

        else if (fromABS_frame.id == 0x104){ // Electric Parking Brake


            #pragma region // Original Processing of 0xAB (ESP Brake Pedal Switch)

                  Susp_CAN_msg0x104_FromABS_buf0_Raw = fromABS_frame.buf[0];
                  Susp_CAN_msg0x104_FromABS_buf1_Raw = fromABS_frame.buf[1];
                  Susp_CAN_msg0x104_FromABS_buf2_Raw = fromABS_frame.buf[2];
                  Susp_CAN_msg0x104_FromABS_buf3_Raw = fromABS_frame.buf[3];
                  Susp_CAN_msg0x104_FromABS_buf4_Raw = fromABS_frame.buf[4];
                  Susp_CAN_msg0x104_FromABS_buf5_Raw = fromABS_frame.buf[5];
                  Susp_CAN_msg0x104_FromABS_buf6_Raw = fromABS_frame.buf[6];
                  Susp_CAN_msg0x104_FromABS_buf7_Raw = fromABS_frame.buf[7];  

                  ParkBrakeStatus_0x104_FromABSCAN = Susp_CAN_msg0x104_FromABS_buf7_Raw;
                  ParkBrakeStatus_0x104_FromABS = ParkBrakeStatus_0x104_FromABSCAN;

                  EPB_01_t epb = parse_EPB_01(fromABS_frame);
                  MQB_EPB_0x104_Clamping_Force = epb.EPB_Clamping_Force;
                  MQB_EPB_0x104_Longitudinal_acceleration = epb.EPB_Longitudinal_acceleration;
                  MQB_EPB_0x104_Delay_requested = epb.EPB_Delay_requested;
                  MQB_EPB_0x104_Status = epb.EPB_Status;
                  MQB_EPB_0x104_Release_Deceleration_Start = epb.EPB_Release_Deceleration_Start;
                  MQB_EPB_0x104_Release_Delay_Request = epb.EPB_Release_Delay_Request;
                // TFTCAN2.write(fromABS_frame);                                       //forwarding from TFTCAN1 (ABS origin) to TFTCAN2 (Gateway)


              #pragma endregion

            #pragma region // Determine LC Bump-In Activation Status and record values at time of activation

                if(LC_BumpIn_ActivationStatus == 1 && LC_BumpIn_TimeSinceActivation < 500 && MQB_ESP05_0x106_BrakePressureMPA_Float > 3 && LC_BumpIn_0x104_CallbackRecorded == 0)  { // Trigger the start of LC BumpIn Activation , which records the initial brake pressure at time of overriding
                        ESP_0x1AB_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf1 = fromABS_frame.buf[1];
                        ESP_0x104_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf2 = fromABS_frame.buf[2];
                        ESP_0x104_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf3 = fromABS_frame.buf[3];
                        ESP_0x104_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf4 = fromABS_frame.buf[4];
                        ESP_0x104_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf5 = fromABS_frame.buf[5];
                        ESP_0x104_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf6 = fromABS_frame.buf[6]; 
                        ESP_0x104_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf7 = fromABS_frame.buf[7];

                        LC_BumpIn_0x104_CallbackRecorded = 1;

                }


                #pragma endregion

            #pragma region // Execute either Unmodified or Modified callback 


              if (LC_BumpIn_ActivationStatus == 0 || LC_BumpIn_0x104_CallbackRecorded == 0) {  // Send unmodified frame if LC_BumpIn_ActivationStatus is NOT Active

                    TFTCAN2.write(fromABS_frame);
                    // Serial.println("Sent UNmodified frame to 0x104...");
                    // printFrame(fromABS_frame, 6);  // 6 - Unmodified Messages forwarding from Can2 (ABS Node) to CAN1 (To Gateway)
                          // debug_print_frame_CAN2_Rx_with_checksum(fromABS_frame);
                          // debug_print_frame_CAN1_Tx_with_checksum(fromABS_frame_mod);

                }

                else if(LC_BumpIn_ActivationStatus == 1 && LC_BumpIn_0x104_CallbackRecorded == 1 ) { // Send modified frame if LC_BumpIn_ActivationStatus IS Active

                          CAN_message_t modFrame_0x104;
                          modFrame_0x104 = fromABS_frame;

                          modFrame_0x104.buf[0] = fromABS_frame.buf[0];
                          modFrame_0x104.buf[1] = fromABS_frame.buf[1];
                          modFrame_0x104.buf[2] = 0x55;
                          modFrame_0x104.buf[3] = 0x05;
                          modFrame_0x104.buf[4] = 0x00;
                          modFrame_0x104.buf[5] = 0x00;
                          modFrame_0x104.buf[6] = 0x00;
                          modFrame_0x104.buf[7] = 0x30;  



                          bool hit = build_frame_with_checksum_0x104(modFrame_0x104.buf);

                          // if (hit) Serial.println("Checksum found in lookup");
                          // else Serial.println("Checksum fallback used");

                          // TFTCAN2.write(modFrame_0x104);
                          TFTCAN2.write(fromABS_frame);


                            // Serial.println("Sent MODIFIED frame to 0x104...");
                            // Serial.print("LC_BumpIn_0x104_CallbackRecorded: "); Serial.print(LC_BumpIn_0x104_CallbackRecorded); Serial.println("\t");                            
                          
                            // printFrame(fromABS_frame, 6);  // 6 - Unmodified Messages forwarding from Can2 (ABS Node) to CAN1 (To Gateway)
                            // printFrame(modFrame_0x104, 4); // 4 - Modified messages from CAN2, sending to CAN1

                            // // // Serial.println("---------------------------------------------------------------------------------");

                      }
                  





                #pragma endregion


              }
        
        else if (fromABS_frame.id == 0x106) {  // Brake Pressure 
              //  printFrame(fromABS_frame, 2);  // 2 - Printing Raw Messages received on Can2
                digitalToggle(52);

            #pragma region // Original Processing of actual Brake Pressure coming from ABS Module

              ESP_05_t esp05 = parse_ESP_05(fromABS_frame);
              MQB_ESP05_0x106_BrakePressureBAR_Float = esp05.ESP_Brake_Pressure;
              MQB_ESP05_0x106_BrakePressureMPA_Float = esp05.ESP_Brake_Pressure / 10;
              MQB_ESP05_0x106_Brake_Pressure = esp05.ESP_Brake_Pressure;
              MQB_ESP05_0x106_Driver_Brakes = esp05.ESP_Driver_Brakes;
              MQB_ESP05_0x106_QBit_Brake_Pressure = esp05.ESP_QBit_Brake_Pressure;
              MQB_ESP05_0x106_Status_Brake_Pressure = esp05.ESP_Status_Brake_Pressure;
              MQB_ESP05_0x106_QBit_Driver_Brakes = esp05.ESP_QBit_Driver_Brakes;
              MQB_ECD_0x106_Brake_Light = esp05.ECD_Brake_Light;
              MQB_ESP05_0x106_Eingr_HL = esp05.ESP_Eingr_HL;
              MQB_ESP05_0x106_Eingr_HR = esp05.ESP_Eingr_HR;
              MQB_ESP05_0x106_Eingr_VL = esp05.ESP_Eingr_VL;
              MQB_ESP05_0x106_Eingr_VR = esp05.ESP_Eingr_VR;


              Susp_CAN_msg0x106_FromABS_buf0_Raw = fromABS_frame.buf[0];
              Susp_CAN_msg0x106_FromABS_buf1_Raw = fromABS_frame.buf[1];
              Susp_CAN_msg0x106_FromABS_buf2_Raw = fromABS_frame.buf[2];
              Susp_CAN_msg0x106_FromABS_buf3_Raw = fromABS_frame.buf[3];
              Susp_CAN_msg0x106_FromABS_buf4_Raw = fromABS_frame.buf[4];
              Susp_CAN_msg0x106_FromABS_buf5_Raw = fromABS_frame.buf[5];
              Susp_CAN_msg0x106_FromABS_buf6_Raw = fromABS_frame.buf[6];
              Susp_CAN_msg0x106_FromABS_buf7_Raw = fromABS_frame.buf[7];  



              Susp_CAN_msg0x106_FromABS_buf0 = Susp_CAN_msg0x106_FromABS_buf0_Raw;
              Susp_CAN_msg0x106_FromABS_buf1 = Susp_CAN_msg0x106_FromABS_buf1_Raw;
              Susp_CAN_msg0x106_FromABS_buf2 = Susp_CAN_msg0x106_FromABS_buf2_Raw;
              Susp_CAN_msg0x106_FromABS_buf3 = Susp_CAN_msg0x106_FromABS_buf3_Raw;
              Susp_CAN_msg0x106_FromABS_buf4 = Susp_CAN_msg0x106_FromABS_buf4_Raw;
              Susp_CAN_msg0x106_FromABS_buf5 = Susp_CAN_msg0x106_FromABS_buf5_Raw;
              Susp_CAN_msg0x106_FromABS_buf6 = Susp_CAN_msg0x106_FromABS_buf6_Raw;
              Susp_CAN_msg0x106_FromABS_buf7 = Susp_CAN_msg0x106_FromABS_buf7_Raw;  


              TFT_MITM_LC_BumpIn_TrueBrakePressureMPA_fromABS_Float = MQB_ESP05_0x106_BrakePressureMPA_Float;
              TFT_MITM_LC_BumpIn_TrueBrakePressureMPA_fromABS_canTx_16bit = TFT_MITM_LC_BumpIn_TrueBrakePressureMPA_fromABS_Float * 100;

                  
          #pragma endregion

            #pragma region // Determine LC Bump-In Activation Status and record values at time of activation

                if(LC_BumpIn_ActivationStatus == 1 && LC_BumpIn_TimeSinceActivation < 500 && MQB_ESP05_0x106_BrakePressureMPA_Float > 1 && LC_BumpIn_0x106_CallbackRecorded == 0)  { // Trigger the start of LC BumpIn Activation , which records the initial brake pressure at time of overriding

                  BrakePedalSwitchStatus = 2;
                        //  Serial.println("BrakePedalSwitchStatus: 2");

                        // BrakePedalSwitchStatus_OverrideButtonStatus = 1;
                        LC_BumpIn_0x106_CallbackRecorded = 1;
                        // LC_BumpIn_ActivationStatus = 1;

                        // BrakePedalSwitchStatus_atTimeof_LCBumpInActivation = BrakePedalSwitchStatus_FromABS;
                        BrakePressureMPA_atTimeof_LCBumpInActivation_Float = MQB_ESP05_0x106_BrakePressureMPA_Float;
              
                        ESP_0x106_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf1 = Susp_CAN_msg0x106_FromABS_buf1;
                        ESP_0x106_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf2 = Susp_CAN_msg0x106_FromABS_buf2;
                        ESP_0x106_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf3 = Susp_CAN_msg0x106_FromABS_buf3;
                        ESP_0x106_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf4 = Susp_CAN_msg0x106_FromABS_buf4;
                        ESP_0x106_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf5 = Susp_CAN_msg0x106_FromABS_buf5;
                        ESP_0x106_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf6 = Susp_CAN_msg0x106_FromABS_buf6;
                        ESP_0x106_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf7 = Susp_CAN_msg0x106_FromABS_buf7;

                      }

                #pragma endregion

            #pragma region // Execute either Unmodified or Modified callback 

              if (LC_BumpIn_ActivationStatus == 0 || LC_BumpIn_0x106_CallbackRecorded == 0 ) {  // Send unmodified frame if LC_BumpIn_ActivationStatus is NOT Active

                    TFTCAN2.write(fromABS_frame);
                    // Serial.println("Sent == 0x106 callback frame to CAN1, LC_BumpIn_ActivationStatus == 0 ");
                    // Serial.println("Sent UNmodified frame to 0x106...");

                            // printFrame(fromABS_frame, 6);  // 6 - Unmodified Messages forwarding from Can2 (ABS Node) to CAN1 (To Gateway)

                }

                else if(LC_BumpIn_ActivationStatus == 1 && LC_BumpIn_0x106_CallbackRecorded == 1 ) { // Send modified frame if LC_BumpIn_ActivationStatus IS Active

                          CAN_message_t modFrame_0x106;
                          modFrame_0x106 = fromABS_frame;

                          modFrame_0x106.buf[0] = fromABS_frame.buf[0];
                          modFrame_0x106.buf[1] = fromABS_frame.buf[1];
                          modFrame_0x106.buf[2] = 0x55;
                          modFrame_0x106.buf[3] = 0x05;
                          modFrame_0x106.buf[4] = 0x00;
                          modFrame_0x106.buf[5] = 0x00;
                          modFrame_0x106.buf[6] = 0x00;
                          modFrame_0x106.buf[7] = 0x30;  



                          bool hit = build_frame_with_checksum_0x106(modFrame_0x106.buf);

                          // if (hit) Serial.println("Checksum found in lookup");
                          // else Serial.println("Checksum fallback used");

                          TFTCAN2.write(modFrame_0x106);
                          // TFTCAN2.write(fromABS_frame);

                            Serial.println("Sent MODIFIED frame to 0x106...");
                            Serial.print("LC_BumpIn_0x106_CallbackRecorded: "); Serial.print(LC_BumpIn_0x106_CallbackRecorded); Serial.println("\t");                            
                      
                            // printFrame(fromABS_frame, 6);  // 6 - Unmodified Messages forwarding from Can2 (ABS Node) to CAN1 (To Gateway)
                            // printFrame(modFrame_0x106, 4); // 4 - Modified messages from CAN2, sending to CAN1

                            // // // Serial.println("---------------------------------------------------------------------------------");

                      }
                  


                #pragma endregion
        
          
       }

        else if (fromABS_frame.id == 0x116){ // ESP_10 Drivetrain/Brakes
              // printFrame(fromABS_frame, 2);                                  //uncomment line to print frames received on TFTCAN2

            #pragma region // Original Processing of 0x116 (ESP Chassis Movements) 


                ESP_10_t esp10 = parse_ESP_10(fromABS_frame);
                MQB_ESP10_0x116_HL_Direction_of_Travel = esp10.ESP_HL_Direction_of_Travel;
                MQB_ESP10_0x116_HR_Direction_of_Travel = esp10.ESP_HR_Direction_of_Travel;
                MQB_ESP10_0x116_VL_Direction_of_Travel = esp10.ESP_VL_Direction_of_Travel;
                MQB_ESP10_0x116_VR_Direction_of_Travel = esp10.ESP_VR_Direction_of_Travel;
                MQB_ESP10_0x116_Path_Impulse_HL = esp10.ESP_Path_Impulse_HL;
                MQB_ESP10_0x116_Path_Impulse_HR = esp10.ESP_Path_Impulse_HR;
                MQB_ESP10_0x116_Path_Impulse_VL = esp10.ESP_Path_Impulse_VL;
                MQB_ESP10_0x116_Path_Impulse_VR = esp10.ESP_Path_Impulse_VR;

            #pragma endregion

            #pragma region // Determine LC Bump-In Activation Status and record values at time of activation

                if(LC_BumpIn_ActivationStatus == 1 && LC_BumpIn_TimeSinceActivation < 500 && MQB_ESP05_0x106_BrakePressureMPA_Float > 3 && LC_BumpIn_0x116_CallbackRecorded == 0)  { // Trigger the start of LC BumpIn Activation , which records the initial brake pressure at time of overriding

                        ESP_0x116_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf1 = fromABS_frame.buf[1];
                        ESP_0x116_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf2 = fromABS_frame.buf[2];
                        ESP_0x116_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf3 = fromABS_frame.buf[3];
                        ESP_0x116_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf4 = fromABS_frame.buf[4];
                        ESP_0x116_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf5 = fromABS_frame.buf[5];
                        ESP_0x116_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf6 = fromABS_frame.buf[6]; 
                        ESP_0x116_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf7 = fromABS_frame.buf[7];

                        LC_BumpIn_0x116_CallbackRecorded = 1;

                }


                #pragma endregion

            #pragma region // Execute either Unmodified or Modified callback 


              if (LC_BumpIn_ActivationStatus == 0 || LC_BumpIn_0x116_CallbackRecorded == 0) {  // Send unmodified frame if LC_BumpIn_ActivationStatus is NOT Active
                    TFTCAN2.write(fromABS_frame);
                    // Serial.println("Sent UNmodified frame to 0x116...");
                    // printFrame(fromABS_frame, 6);  // 6 - Unmodified Messages forwarding from Can2 (ABS Node) to CAN1 (To Gateway)

                }

                else if(LC_BumpIn_ActivationStatus == 1 && LC_BumpIn_0x116_CallbackRecorded == 1 ) { // Send modified frame if LC_BumpIn_ActivationStatus IS Active

                          CAN_message_t modFrame_0x116;
                          modFrame_0x116 = fromABS_frame;

                          // modFrame_0x116.buf[0] = fromABS_frame.buf[0];
                          // modFrame_0x116.buf[1] = fromABS_frame.buf[1];
                          // modFrame_0x116.buf[2] = ESP_0x116_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf2;
                          // modFrame_0x116.buf[3] = ESP_0x116_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf3;
                          // modFrame_0x116.buf[4] = ESP_0x116_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf4;
                          // modFrame_0x116.buf[5] = ESP_0x116_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf5;
                          // modFrame_0x116.buf[6] = ESP_0x116_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf6;
                          // modFrame_0x116.buf[7] = ESP_0x116_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf7;

                          modFrame_0x116.buf[0] = fromABS_frame.buf[0];
                          modFrame_0x116.buf[1] = fromABS_frame.buf[1];
                          modFrame_0x116.buf[2] = fromABS_frame.buf[2];
                          modFrame_0x116.buf[3] = fromABS_frame.buf[3];
                          modFrame_0x116.buf[4] = fromABS_frame.buf[4];
                          modFrame_0x116.buf[5] = fromABS_frame.buf[5];
                          modFrame_0x116.buf[6] = fromABS_frame.buf[6];
                          modFrame_0x116.buf[7] = fromABS_frame.buf[7];


                          bool hit = build_frame_with_checksum_0x116(modFrame_0x116.buf);

                          // if (hit) Serial.println("Checksum found in lookup");
                          // else Serial.println("Checksum fallback used");

                          // TFTCAN2.write(modFrame_0x116);
                          TFTCAN2.write(fromABS_frame);

                            // Serial.println("Sent MODIFIED frame to 0x116...");
                            // Serial.print("LC_BumpIn_0x116_CallbackRecorded: "); Serial.print(LC_BumpIn_0x116_CallbackRecorded); Serial.println("\t");                            
                          
                            // printFrame(fromABS_frame, 6);  // 6 - Unmodified Messages forwarding from Can2 (ABS Node) to CAN1 (To Gateway)
                            // printFrame(modFrame_0x116, 4); // 4 - Modified messages from CAN2, sending to CAN1

                            // // // Serial.println("---------------------------------------------------------------------------------");

                      }
                  



                #pragma endregion



            }

        else if (fromABS_frame.id == 0x11E){ // ESP_08Chassis Movements - velocity, direction of travel, etc
              // printFrame(fromABS_frame, 2);                                  //uncomment line to print frames received on TFTCAN2


            #pragma region // Original Processing of 0x101 (ESP Chassis Movements) 


                ESP_08_t esp08 = parse_ESP_08(fromABS_frame);
                MQB_ESP08_0x11E_Slipping_Stillstand = esp08.ESP_Slipping_Stillstand;
                MQB_ESP08_0x11E_StandStillPhase_Exhausted = esp08.ESP_StandStillPhase_Exhausted;
                MQB_ESP08_0x11E_VehicleSpeed_ref_Direction_Of_Travel = esp08.ESP_v_ref_Direction_Of_Travel;
                MQB_ESP08_0x11E_VehicleSpeed_ref = esp08.ESP_v_ref;
                MQB_ESC_0x11E_Brake_Pressure_Gradient = esp08.ESC_Brake_Pressure_Gradient;

            #pragma endregion

            #pragma region // Determine LC Bump-In Activation Status and record values at time of activation

                if(LC_BumpIn_ActivationStatus == 1 && LC_BumpIn_TimeSinceActivation < 500 && MQB_ESP05_0x106_BrakePressureMPA_Float > 3 && LC_BumpIn_0x11E_CallbackRecorded == 0)  { // Trigger the start of LC BumpIn Activation , which records the initial brake pressure at time of overriding

                        // ESP_0x1AB_Statuses_atTimeof_LCBumpInActivation_Int_buf1 = fromABS_frame.buf[1];
                        ESP_0x11E_Statuses_atTimeof_LCBumpInActivation_Int_buf2 = fromABS_frame.buf[2];
                        ESP_0x11E_Statuses_atTimeof_LCBumpInActivation_Int_buf3 = fromABS_frame.buf[3];
                        ESP_0x11E_Statuses_atTimeof_LCBumpInActivation_Int_buf4 = fromABS_frame.buf[4];
                        ESP_0x11E_Statuses_atTimeof_LCBumpInActivation_Int_buf5 = fromABS_frame.buf[5];
                        ESP_0x11E_Statuses_atTimeof_LCBumpInActivation_Int_buf6 = fromABS_frame.buf[6]; 
                        ESP_0x11E_Statuses_atTimeof_LCBumpInActivation_Int_buf7 = fromABS_frame.buf[7];

                        LC_BumpIn_0x11E_CallbackRecorded = 1;

                }


                #pragma endregion

            #pragma region // Execute either Unmodified or Modified callback 


              if (LC_BumpIn_ActivationStatus == 0 || LC_BumpIn_0x11E_CallbackRecorded == 0) {  // Send unmodified frame if LC_BumpIn_ActivationStatus is NOT Active

                    TFTCAN2.write(fromABS_frame);
                    // Serial.println("Sent UNmodified frame to 0x11E...");
                    // printFrame(fromABS_frame, 6);  // 6 - Unmodified Messages forwarding from Can2 (ABS Node) to CAN1 (To Gateway)
                          // debug_print_frame_CAN2_Rx_with_checksum(fromABS_frame);
                          // debug_print_frame_CAN1_Tx_with_checksum(fromABS_frame_mod);

                }

                else if(LC_BumpIn_ActivationStatus == 1 && LC_BumpIn_0x11E_CallbackRecorded == 1 ) { // Send modified frame if LC_BumpIn_ActivationStatus IS Active

                          CAN_message_t modFrame_0x11E;
                          modFrame_0x11E = fromABS_frame;

                          modFrame_0x11E.buf[0] = fromABS_frame.buf[0];
                          modFrame_0x11E.buf[1] = fromABS_frame.buf[1];
                          modFrame_0x11E.buf[2] = 0x00;
                          modFrame_0x11E.buf[3] = 0x00;
                          modFrame_0x11E.buf[4] = 0x00;
                          modFrame_0x11E.buf[5] = 0x00;
                          modFrame_0x11E.buf[6] = 0x00;
                          modFrame_0x11E.buf[7] = 0x00;  



                          bool hit = build_frame_with_checksum_0x11E(modFrame_0x11E.buf);

                          // if (hit) Serial.println("Checksum found in lookup");
                          // else Serial.println("Checksum fallback used");

                          // TFTCAN2.write(modFrame_0x11E);
                          TFTCAN2.write(fromABS_frame);

                            // Serial.println("Sent MODIFIED frame to 0x11E...");
                            // Serial.print("LC_BumpIn_0x11E_CallbackRecorded: "); Serial.print(LC_BumpIn_0x11E_CallbackRecorded); Serial.println("\t");                            
                         
                            // printFrame(fromABS_frame, 6);  // 6 - Unmodified Messages forwarding from Can2 (ABS Node) to CAN1 (To Gateway)
                            // printFrame(modFrame_0x11E, 4); // 4 - Modified messages from CAN2, sending to CAN1

                            // // // Serial.println("---------------------------------------------------------------------------------");

                      }
                  



                #pragma endregion




            }

        else if (fromABS_frame.id == 0x1AB){ // Brake Pedal Switch

          
            #pragma region // Original Processing of 0xAB (ESP Brake Pedal Switch)

                  Susp_CAN_msg0x1AB_FromABS_buf0_Raw = fromABS_frame.buf[0];
                  Susp_CAN_msg0x1AB_FromABS_buf1_Raw = fromABS_frame.buf[1];
                  Susp_CAN_msg0x1AB_FromABS_buf2_Raw = fromABS_frame.buf[2];
                  Susp_CAN_msg0x1AB_FromABS_buf3_Raw = fromABS_frame.buf[3];
                  Susp_CAN_msg0x1AB_FromABS_buf4_Raw = fromABS_frame.buf[4];
                  Susp_CAN_msg0x1AB_FromABS_buf5_Raw = fromABS_frame.buf[5];
                  Susp_CAN_msg0x1AB_FromABS_buf6_Raw = fromABS_frame.buf[6];
                  Susp_CAN_msg0x1AB_FromABS_buf7_Raw = fromABS_frame.buf[7];  

                BrakeSwitch_0x1AB_CAN_FromABS = Susp_CAN_msg0x1AB_FromABS_buf6_Raw;
                BrakeSwitch_0x1AB_FromABS = BrakeSwitch_0x1AB_CAN_FromABS; // When pedal is not pressed, byte6 is 0x04.  When pedal is pressed, byte6 is 0x05

                if(BrakeSwitch_0x1AB_FromABS == 4) {
                BrakePedalSwitchStatus_FromABS = 0;
                }
                else if(BrakeSwitch_0x1AB_FromABS == 5) {
                BrakePedalSwitchStatus_FromABS = 1;
                }
              // Serial.print("BrakeSwitch_0x1AB_FromABS: ");  Serial.print(BrakeSwitch_0x1AB_FromABS);  
              // Serial.println();
              // Serial.print("BrakePedalSwitchStatus_FromABS: ");  Serial.print(BrakePedalSwitchStatus_FromABS);  
              // Serial.println();
                // TFTCAN2.write(fromABS_frame);                                       //forwarding from TFTCAN1 (ABS origin) to TFTCAN2 (Gateway)

              #pragma endregion

            #pragma region // Determine LC Bump-In Activation Status and record values at time of activation

                if(LC_BumpIn_ActivationStatus == 1 && LC_BumpIn_TimeSinceActivation < 500 && MQB_ESP05_0x106_BrakePressureMPA_Float > 3 && LC_BumpIn_0x1AB_CallbackRecorded == 0)  { // Trigger the start of LC BumpIn Activation , which records the initial brake pressure at time of overriding

                        ESP_0x1AB_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf1 = fromABS_frame.buf[1];
                        ESP_0x1AB_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf2 = fromABS_frame.buf[2];
                        ESP_0x1AB_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf3 = fromABS_frame.buf[3]; // Should be 0x2 when brake is pressed
                        ESP_0x1AB_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf4 = fromABS_frame.buf[4];
                        ESP_0x1AB_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf5 = fromABS_frame.buf[5];
                        ESP_0x1AB_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf6 = fromABS_frame.buf[6]; // Shoiuld be 0x5 when brake is pressed
                        ESP_0x1AB_BrakeStatuses_atTimeof_LCBumpInActivation_Int_buf7 = fromABS_frame.buf[7];

                        LC_BumpIn_0x1AB_CallbackRecorded = 1;

                }


                #pragma endregion

            #pragma region // Execute either Unmodified or Modified callback 

              if (LC_BumpIn_ActivationStatus == 0 || LC_BumpIn_0x1AB_CallbackRecorded == 0 ) {  // Send unmodified frame if LC_BumpIn_ActivationStatus is NOT Active

                    TFTCAN2.write(fromABS_frame);
                    // Serial.println("Sent UNmodified frame to 0x1AB...");
                    // printFrame(fromABS_frame, 6);  // 6 - Unmodified Messages forwarding from Can2 (ABS Node) to CAN1 (To Gateway)

                }
          
                else if(LC_BumpIn_ActivationStatus == 1 && LC_BumpIn_0x1AB_CallbackRecorded == 1 ) { // Send modified frame if LC_BumpIn_ActivationStatus IS Active

                          CAN_message_t modFrame_0x1AB;
                          modFrame_0x1AB = fromABS_frame;

                          modFrame_0x1AB.buf[0] = fromABS_frame.buf[0];
                          modFrame_0x1AB.buf[1] = fromABS_frame.buf[1];
                          modFrame_0x1AB.buf[2] = 0x00;
                          modFrame_0x1AB.buf[3] = 0x02;
                          modFrame_0x1AB.buf[4] = 0x00;
                          modFrame_0x1AB.buf[5] = 0x00;
                          modFrame_0x1AB.buf[6] = 0x05;
                          modFrame_0x1AB.buf[7] = 0x00;  



                          bool hit = build_frame_with_checksum_0x1AB(modFrame_0x1AB.buf);

                          // if (hit) Serial.println("Checksum found in lookup");
                          // else Serial.println("Checksum fallback used");

                          TFTCAN2.write(modFrame_0x1AB);
                          // TFTCAN2.write(fromABS_frame);

                          // Serial.println("Sent MODIFIED frame to 0x1AB...");
                            // Serial.print("LC_BumpIn_0x1AB_CallbackRecorded: "); Serial.print(LC_BumpIn_0x1AB_CallbackRecorded); Serial.println("\t");                            
                        
                            // printFrame(fromABS_frame, 6);  // 6 - Unmodified Messages forwarding from Can2 (ABS Node) to CAN1 (To Gateway)                         
                            // printFrame(modFrame_0x1AB, 4); // 4 - Modified messages from CAN2, sending to CAN1

                            // // // Serial.println("---------------------------------------------------------------------------------");

                      }
                  

                #pragma endregion



              }

        else if (fromABS_frame.id == 0x31B) { // ESP_24 - Vehicle speed and wheel rotation counter

            #pragma region // Original Processing of 0x31B (ESP Chassis Movements) 

            ESP_24_t esp24 = parse_ESP_24(fromABS_frame);
            // MQB_ESP24_0x31B_CHECKSUM ;           
            // MQB_ESP24_0x31B_COUNTER ;            
            MQB_ESP24_0x31B_VehicleSpeed = esp24.ESP24_VehicleSpeed;       
            MQB_ESP24_0x31B_ESP24UnknownByte4 = esp24.ESP24_UnknownByte4;  
            MQB_ESP24_0x31B_WheelRotationCounter = esp24.ESP24_WheelRotationCounter;

            #pragma endregion

            #pragma region // Determine LC Bump-In Activation Status and record values at time of activation

                if(LC_BumpIn_ActivationStatus == 1 && LC_BumpIn_TimeSinceActivation < 500 && MQB_ESP05_0x106_BrakePressureMPA_Float > 3 && LC_BumpIn_0x31B_CallbackRecorded == 0)  { // Trigger the start of LC BumpIn Activation , which records the initial brake pressure at time of overriding

                        ESP_0x31B_Statuses_atTimeof_LCBumpInActivation_Int_buf1 = fromABS_frame.buf[1];
                        ESP_0x31B_Statuses_atTimeof_LCBumpInActivation_Int_buf2 = fromABS_frame.buf[2];
                        ESP_0x31B_Statuses_atTimeof_LCBumpInActivation_Int_buf3 = fromABS_frame.buf[3];
                        ESP_0x31B_Statuses_atTimeof_LCBumpInActivation_Int_buf4 = fromABS_frame.buf[4];
                        ESP_0x31B_Statuses_atTimeof_LCBumpInActivation_Int_buf5 = fromABS_frame.buf[5];
                        ESP_0x31B_Statuses_atTimeof_LCBumpInActivation_Int_buf6 = fromABS_frame.buf[6]; 
                        ESP_0x31B_Statuses_atTimeof_LCBumpInActivation_Int_buf7 = fromABS_frame.buf[7];

                        LC_BumpIn_0xFD_CallbackRecorded = 1;

                }


                #pragma endregion

            #pragma region // Execute either Unmodified or Modified callback 

              if (LC_BumpIn_ActivationStatus == 0 || LC_BumpIn_0x31B_CallbackRecorded == 0) {  // Send unmodified frame if LC_BumpIn_ActivationStatus is NOT Active

                    TFTCAN2.write(fromABS_frame);
                    // Serial.println("Sent UNmodified frame to 0xFD...");
                    // printFrame(fromABS_frame, 6);  // 6 - Unmodified Messages forwarding from Can2 (ABS Node) to CAN1 (To Gateway)

                }

                else if(LC_BumpIn_ActivationStatus == 1 && LC_BumpIn_0x31B_CallbackRecorded == 1 ) { // Send modified frame if LC_BumpIn_ActivationStatus IS Active

                          CAN_message_t modFrame_0x31B;
                          modFrame_0x31B = fromABS_frame;

                          modFrame_0x31B.buf[0] = fromABS_frame.buf[0];
                          modFrame_0x31B.buf[1] = fromABS_frame.buf[1];
                          modFrame_0x31B.buf[2] = 0x00;
                          modFrame_0x31B.buf[3] = 0x00;
                          modFrame_0x31B.buf[4] = 0x00;
                          modFrame_0x31B.buf[5] = ESP_0x31B_Statuses_atTimeof_LCBumpInActivation_Int_buf5;
                          modFrame_0x31B.buf[6] = ESP_0x31B_Statuses_atTimeof_LCBumpInActivation_Int_buf6;
                          modFrame_0x31B.buf[7] = 0x00;



                          bool hit = build_frame_with_checksum_0x31B(modFrame_0x31B.buf);

                          // if (hit) Serial.println("Checksum found in lookup");
                          // else Serial.println("Checksum fallback used");

                          // TFTCAN2.write(modFrame_0x31B);
                          TFTCAN2.write(fromABS_frame);

                            // Serial.println("Sent MODIFIED frame to 0x31B...");
                            // Serial.print("LC_BumpIn_0x31B_CallbackRecorded: "); Serial.print(LC_BumpIn_0x31B_CallbackRecorded); Serial.println("\t");                            
                         
                            // printFrame(fromABS_frame, 6);  // 6 - Unmodified Messages forwarding from Can2 (ABS Node) to CAN1 (To Gateway)
                            // printFrame(modFrame_0x31B, 4); // 4 - Modified messages from CAN2, sending to CAN1
                            // // Serial.println("---------------------------------------------------------------------------------");

                      }
                  


                #pragma endregion
    

        }

        else if (fromABS_frame.id == 0x392) { // ESP Wheel statuses
          //  printFrame(fromABS_frame, 2);                                  //uncomment line to print frames received on TFTCAN2


          #pragma region // Original Processing of 0x101 (ESP Chassis Movements) 


              ESP_07_t esp07 = parse_ESP_07(fromABS_frame);
              MQB_ESP07_0x392_RTA_HL = esp07.ESP_RTA_HL;
              MQB_ESP07_0x392_RTA_HR = esp07.ESP_RTA_HR;
              MQB_ESP07_0x392_RTA_VR = esp07.ESP_RTA_VR;
              MQB_ESP07_0x392_OBD_Status = esp07.ESP_OBD_Status;
              MQB_OBD_0x392_Wheel_Sensor_Error_HL = esp07.OBD_Wheel_Sensor_Error_HL;
              MQB_OBD_0x392_Wheel_Sensor_Error_HR = esp07.OBD_Wheel_Sensor_Error_HR;
              MQB_OBD_0x392_Wheel_Sensor_Error_VL = esp07.OBD_Wheel_Sensor_Error_VL;
              MQB_OBD_0x392_Wheel_Sensor_Error_VR = esp07.OBD_Wheel_Sensor_Error_VR;
              MQB_ESP07_0x392_MKB_Status = esp07.ESP_MKB_Status;
              MQB_ESP07_0x392_Quattro_Drie = esp07.ESP_Quattro_Drie;

            #pragma endregion

          #pragma region // Determine LC Bump-In Activation Status and record values at time of activation

                if(LC_BumpIn_ActivationStatus == 1 && LC_BumpIn_TimeSinceActivation < 500 && MQB_ESP05_0x106_BrakePressureMPA_Float > 3 && LC_BumpIn_0x392_CallbackRecorded == 0)  { // Trigger the start of LC BumpIn Activation , which records the initial brake pressure at time of overriding

                        ESP_0x392_Statuses_atTimeof_LCBumpInActivation_Int_buf1 = fromABS_frame.buf[1];
                        ESP_0x392_Statuses_atTimeof_LCBumpInActivation_Int_buf2 = fromABS_frame.buf[2];
                        ESP_0x392_Statuses_atTimeof_LCBumpInActivation_Int_buf3 = fromABS_frame.buf[3];
                        ESP_0x392_Statuses_atTimeof_LCBumpInActivation_Int_buf4 = fromABS_frame.buf[4];
                        ESP_0x392_Statuses_atTimeof_LCBumpInActivation_Int_buf5 = fromABS_frame.buf[5];
                        ESP_0x392_Statuses_atTimeof_LCBumpInActivation_Int_buf6 = fromABS_frame.buf[6]; 
                        ESP_0x392_Statuses_atTimeof_LCBumpInActivation_Int_buf7 = fromABS_frame.buf[7];

                        LC_BumpIn_0x392_CallbackRecorded = 1;

                }


                #pragma endregion

          #pragma region // Execute either Unmodified or Modified callback 


              if (LC_BumpIn_ActivationStatus == 0 || LC_BumpIn_0x392_CallbackRecorded == 0) {  // Send unmodified frame if LC_BumpIn_ActivationStatus is NOT Active

                    TFTCAN2.write(fromABS_frame);
                    // Serial.println("Sent UNmodified frame to 0x392...");
                    // printFrame(fromABS_frame, 6);  // 6 - Unmodified Messages forwarding from Can2 (ABS Node) to CAN1 (To Gateway)

                }

                else if(LC_BumpIn_ActivationStatus == 1 && LC_BumpIn_0x392_CallbackRecorded == 1 ) { // Send modified frame if LC_BumpIn_ActivationStatus IS Active

                          CAN_message_t modFrame_0x392;
                          modFrame_0x392 = fromABS_frame;

                          modFrame_0x392.buf[0] = fromABS_frame.buf[0];
                          modFrame_0x392.buf[1] = fromABS_frame.buf[1];
                          modFrame_0x392.buf[2] = ESP_0x392_Statuses_atTimeof_LCBumpInActivation_Int_buf2;
                          modFrame_0x392.buf[3] = ESP_0x392_Statuses_atTimeof_LCBumpInActivation_Int_buf3;
                          modFrame_0x392.buf[4] = ESP_0x392_Statuses_atTimeof_LCBumpInActivation_Int_buf4;
                          modFrame_0x392.buf[5] = ESP_0x392_Statuses_atTimeof_LCBumpInActivation_Int_buf5;
                          modFrame_0x392.buf[6] = ESP_0x392_Statuses_atTimeof_LCBumpInActivation_Int_buf6;
                          modFrame_0x392.buf[7] = ESP_0x392_Statuses_atTimeof_LCBumpInActivation_Int_buf7;



                          bool hit = build_frame_with_checksum_0x392(modFrame_0x392.buf);

                          // if (hit) Serial.println("Checksum found in lookup");
                          // else Serial.println("Checksum fallback used");

                          // TFTCAN2.write(modFrame_0x392);
                          TFTCAN2.write(fromABS_frame);

                            // Serial.println("Sent MODIFIED frame to 0x392...");
                            // Serial.print("LC_BumpIn_0x392_CallbackRecorded: "); Serial.print(LC_BumpIn_0x392_CallbackRecorded); Serial.println("\t");                            
                           
                            // printFrame(fromABS_frame, 6);  // 6 - Unmodified Messages forwarding from Can2 (ABS Node) to CAN1 (To Gateway)
                            // printFrame(modFrame_0x392, 4); // 4 - Modified messages from CAN2, sending to CAN1
                            // // // Serial.println("---------------------------------------------------------------------------------");

                      }
                  




                #pragma endregion



        }


      #pragma endregion


      #pragma region // Frames with no checksum bytes  (Wheel Speed Sensors)

        else if (fromABS_frame.id == 0xB2) { // Wheel Speed Sensors
        // printFrame(fromABS_frame, 2);                                  //uncomment line to print frames received on TFTCAN2

          #pragma region // Original Processing of 0xB2 (Wheel Speeds) 

            ESP_19_t esp19 = parse_ESP_19(fromABS_frame);
            MQB_ESP_0xB2_HL_Wheel_Speed_02 = esp19.ESP_HL_Wheel_Speed_02;
            MQB_ESP_0xB2_HR_Wheel_Speed_02 = esp19.ESP_HR_Wheel_Speed_02;
            MQB_ESP_0xB2_VL_Wheel_Speed_02 = esp19.ESP_VL_Wheel_Speed_02;
            MQB_ESP_0xB2_VR_Wheel_Speed_02 = esp19.ESP_VR_Wheel_Speed_02;


            WhlSpdRL_fromABS_Float_Raw = MQB_ESP_0xB2_HR_Wheel_Speed_02;
            WhlSpdRR_fromABS_Float_Raw = MQB_ESP_0xB2_HR_Wheel_Speed_02;
            WhlSpdFL_fromABS_Float_Raw = MQB_ESP_0xB2_VL_Wheel_Speed_02;
            WhlSpdFR_fromABS_Float_Raw = MQB_ESP_0xB2_VR_Wheel_Speed_02;

            WhlSpdRL_fromABS_Int = WhlSpdRL_fromABS_Float_Raw;
            WhlSpdRR_fromABS_Int = WhlSpdRR_fromABS_Float_Raw;
            WhlSpdFL_fromABS_Int = WhlSpdFL_fromABS_Float_Raw;
            WhlSpdFR_fromABS_Int = WhlSpdFR_fromABS_Float_Raw;


            TFT_MITM_LC_BumpIn_TrueWhlSpdRR_fromABS_canTx_16bit = ( WhlSpdRR_fromABS_Float_Raw * 1000 ) ;
          

          #pragma endregion

            #pragma region // Determine LC Bump-In Activation Status and record values at time of activation

                if(LC_BumpIn_ActivationStatus == 1 && LC_BumpIn_TimeSinceActivation < 100 && MQB_ESP05_0x106_BrakePressureMPA_Float > 1 && LC_BumpIn_0xB2_CallbackRecorded == 0)  { // Trigger the start of LC BumpIn Activation , which records the initial brake pressure at time of overriding

                        ESP_0xB2_WheelSpeedStatuses_atTimeof_LCBumpInActivation_Int_buf0 = fromABS_frame.buf[0];
                        ESP_0xB2_WheelSpeedStatuses_atTimeof_LCBumpInActivation_Int_buf1 = fromABS_frame.buf[1];
                        ESP_0xB2_WheelSpeedStatuses_atTimeof_LCBumpInActivation_Int_buf2 = fromABS_frame.buf[2];
                        ESP_0xB2_WheelSpeedStatuses_atTimeof_LCBumpInActivation_Int_buf3 = fromABS_frame.buf[3];
                        ESP_0xB2_WheelSpeedStatuses_atTimeof_LCBumpInActivation_Int_buf4 = fromABS_frame.buf[4];
                        ESP_0xB2_WheelSpeedStatuses_atTimeof_LCBumpInActivation_Int_buf5 = fromABS_frame.buf[5];
                        ESP_0xB2_WheelSpeedStatuses_atTimeof_LCBumpInActivation_Int_buf6 = fromABS_frame.buf[6]; 
                        ESP_0xB2_WheelSpeedStatuses_atTimeof_LCBumpInActivation_Int_buf7 = fromABS_frame.buf[7];

                        LC_BumpIn_0xB2_CallbackRecorded = 1;

                }


                #pragma endregion

            #pragma region // Execute either Unmodified or Modified callback 

              if (LC_BumpIn_ActivationStatus == 0 || LC_BumpIn_0xB2_CallbackRecorded == 0) {  // Send unmodified frame if LC_BumpIn_ActivationStatus is NOT Active

                    TFTCAN2.write(fromABS_frame);
                    // Serial.println("Sent UNmodified frame to 0xB2...");
                    // printFrame(fromABS_frame, 6);  // 6 - Unmodified Messages forwarding from Can2 (ABS Node) to CAN1 (To Gateway)

                }

                else if(LC_BumpIn_ActivationStatus == 1 && LC_BumpIn_0xB2_CallbackRecorded == 1) { // Send modified frame if LC_BumpIn_ActivationStatus IS Active

                          fromABS_frame_mod = fromABS_frame;

                          fromABS_frame_mod.buf[0] = ESP_0xB2_WheelSpeedStatuses_atTimeof_LCBumpInActivation_Int_buf0;
                          fromABS_frame_mod.buf[1] = ESP_0xB2_WheelSpeedStatuses_atTimeof_LCBumpInActivation_Int_buf1;
                          fromABS_frame_mod.buf[2] = ESP_0xB2_WheelSpeedStatuses_atTimeof_LCBumpInActivation_Int_buf2;
                          fromABS_frame_mod.buf[3] = ESP_0xB2_WheelSpeedStatuses_atTimeof_LCBumpInActivation_Int_buf3;
                          fromABS_frame_mod.buf[4] = ESP_0xB2_WheelSpeedStatuses_atTimeof_LCBumpInActivation_Int_buf4;
                          fromABS_frame_mod.buf[5] = ESP_0xB2_WheelSpeedStatuses_atTimeof_LCBumpInActivation_Int_buf5;
                          fromABS_frame_mod.buf[6] = ESP_0xB2_WheelSpeedStatuses_atTimeof_LCBumpInActivation_Int_buf6;
                          fromABS_frame_mod.buf[7] = ESP_0xB2_WheelSpeedStatuses_atTimeof_LCBumpInActivation_Int_buf7;



                          
                            TFTCAN2.write(fromABS_frame_mod);
                            // Serial.println("Sent MODIFIED frame to 0xB2...");
                            // printFrame(fromABS_frame_mod, 4); // 4 - Modified messages from CAN2, sending to CAN1

                      }
                  


                #pragma endregion
     




        }

        #pragma endregion

      
        else { // Execute FIFO Callback of Unmodified frames

            TFTCAN2.write(fromABS_frame);
            // printFrame(fromABS_frame, 6);  // 6 - Unmodified Messages forwarding from Can2 (ABS Node) to CAN1 (To Gateway)

        }


    #pragma region // Determine LC Bump-In Activation Status (using CC Stalk from CAN1, and then using BrakePressure from CAN2)

            if (LC_BumpIn_ActivationStatus == 0 && MQB_ESP05_0x106_BrakePressureMPA_Float > 1 && LC_BumpIn_AcceptableConditions == 1 && LC_BumpIn_ActivationButton == 1 ) {
              
                  LC_BumpIn_ActivationStatus = 1;
                  LC_BumpIn_TimeSinceActivation = 0;
                  Serial.println("LC_BumpIn_ActivationStatus activating....");

                  
              }

              else if( LC_BumpIn_ActivationButton == 0 ) {

                  LC_BumpIn_ActivationStatus = 0;
                  // LC_BumpIn_TimeSinceActivation = 0;
                  LC_BumpIn_0x101_CallbackRecorded = 0;
                  LC_BumpIn_0x104_CallbackRecorded = 0;
                  LC_BumpIn_0x106_CallbackRecorded = 0;
                  LC_BumpIn_0x116_CallbackRecorded = 0;
                  LC_BumpIn_0x11E_CallbackRecorded = 0;
                  LC_BumpIn_0x1AB_CallbackRecorded = 0;
                  LC_BumpIn_0x31B_CallbackRecorded = 0;
                  LC_BumpIn_0x392_CallbackRecorded = 0;
                  LC_BumpIn_0x3C8_CallbackRecorded = 0;
                  LC_BumpIn_0xB2_CallbackRecorded = 0;
                  LC_BumpIn_0xFD_CallbackRecorded = 0;
                }

              if(LC_BumpIn_ActivationButton == 0 ) {

                  LC_BumpIn_ActivationStatus = 0;
                  // LC_BumpIn_TimeSinceActivation = 0;

                  LC_BumpIn_0x101_CallbackRecorded = 0;
                  LC_BumpIn_0x104_CallbackRecorded = 0;
                  LC_BumpIn_0x106_CallbackRecorded = 0;
                  LC_BumpIn_0x116_CallbackRecorded = 0;
                  LC_BumpIn_0x11E_CallbackRecorded = 0;
                  LC_BumpIn_0x1AB_CallbackRecorded = 0;
                  LC_BumpIn_0x31B_CallbackRecorded = 0;
                  LC_BumpIn_0x392_CallbackRecorded = 0;
                  LC_BumpIn_0x3C8_CallbackRecorded = 0;
                  LC_BumpIn_0xB2_CallbackRecorded = 0;
                  LC_BumpIn_0xFD_CallbackRecorded = 0;
                }

        


      // if(WhlSpdRR_fromABS_Int < 10) { // Initial Determination of "Acceptable Conditions" for LC BumpIn Activation

      //     LC_BumpIn_AcceptableConditions = 1;

      // }

      //   else {
      //     LC_BumpIn_AcceptableConditions = 0;
      //     }


            
      #pragma endregion


    }



}


  // Poll TFTCAN2 (Wired to Gateway)
void loop_TFTCAN2_poll() {
  CAN_message_t fromGateway_frame;
  CAN_message_t fromGateway_frame_mod;

  // CAN1 → CAN2
  while (TFTCAN2.read(fromGateway_frame)) {
    TFTCAN1.write(fromGateway_frame);


    // fromGateway_frame_mod = fromGateway_frame;
    // user_modify_payload(fromGateway_frame.id, fromGateway_frame.buf, false);

    // uint8_t payload7[7];
    // extract_payload7(fromGateway_frame.buf, payload7);

    // uint8_t newb0;
    // if (checksum_update_byte0(fromGateway_frame.id, payload7, &newb0))
    //   fromGateway_frame.buf[0] = newb0;



// ---------------------------------------------------------------------------------------------------------------

#pragma region // Process incoming CAN frames 



        if(fromGateway_frame.id == 0x120){ // Cruise stalk static position
        // // printFrame(fromGateway_frame, 3);                                  //uncomment line to print frames received on TFTCAN2

        //       Susp_CAN_msg0x120_FromGateway_buf0_Raw = fromGateway_frame.buf[0];
        //       Susp_CAN_msg0x120_FromGateway_buf1_Raw = fromGateway_frame.buf[1];
        //       Susp_CAN_msg0x120_FromGateway_buf2_Raw = fromGateway_frame.buf[2];
        //       Susp_CAN_msg0x120_FromGateway_buf3_Raw = fromGateway_frame.buf[3];
        //       Susp_CAN_msg0x120_FromGateway_buf4_Raw = fromGateway_frame.buf[4];
        //       Susp_CAN_msg0x120_FromGateway_buf5_Raw = fromGateway_frame.buf[5];
        //       Susp_CAN_msg0x120_FromGateway_buf6_Raw = fromGateway_frame.buf[6];
        //       Susp_CAN_msg0x120_FromGateway_buf7_Raw = fromGateway_frame.buf[7];  

        //        cruiseStalkStaticPosition_0x120_FromGatewayCAN = Susp_CAN_msg0x120_FromGateway_buf3_Raw;
        //        cruiseStalkStaticPosition_0x120_FromGateway = cruiseStalkStaticPosition_0x120_FromGatewayCAN;


        //        if(cruiseStalkStaticPosition_0x120_FromGateway == 0 ) { // 0x06/ Dec 6 = cruise Stalk in Static ON position, no button pressed
        //           CruiseStalk_ON_FromGateway = 0;
        //           CruiseStalk_OFF_FromGateway = 1;

        //           LC_BumpIn_ActivationButton = 1;

        //           }

        //        else{ //  = cruise Stalk in Static ON position, no button pressed
        //           CruiseStalk_ON_FromGateway = 1;
        //           CruiseStalk_OFF_FromGateway = 0;

        //           LC_BumpIn_ActivationButton = 0;

        //           }




              // TFTCAN1.write(fromGateway_frame);                                       //forwarding from TFTCAN2 (Gateway origin) to TFTCAN1 (ABS Node)

            }

        else if(fromGateway_frame.id == 0x391){

                // Susp_CAN_msg0x391_FromGateway_buf0_Raw = fromGateway_frame.buf[0];
                // Susp_CAN_msg0x391_FromGateway_buf1_Raw = fromGateway_frame.buf[1];
                // Susp_CAN_msg0x391_FromGateway_buf2_Raw = fromGateway_frame.buf[2];
                // Susp_CAN_msg0x391_FromGateway_buf3_Raw = fromGateway_frame.buf[3];
                // Susp_CAN_msg0x391_FromGateway_buf4_Raw = fromGateway_frame.buf[4];
                // Susp_CAN_msg0x391_FromGateway_buf5_Raw = fromGateway_frame.buf[5];
                // Susp_CAN_msg0x391_FromGateway_buf6_Raw = fromGateway_frame.buf[6];
                // Susp_CAN_msg0x391_FromGateway_buf7_Raw = fromGateway_frame.buf[7];  


            //   ThrottleTPS_0x391_FromGatewayCAN = Susp_CAN_msg0x391_FromGateway_buf2_Raw;
            //   ThrottleTPS_0x391_FromGateway_Float = (ThrottleTPS_0x391_FromGatewayCAN * 0.39215686275 ) ;

            //   ThrottleTPS_0x391_FromGateway_Mapped_Float = map(ThrottleTPS_0x391_FromGateway_Float,10,85,0,100);
            //   ThrottleTPS_0x391_FromGateway_Mapped_Int = ThrottleTPS_0x391_FromGateway_Mapped_Float;
            //   ThrottleTPS_0x391_FromGateway_Mapped_Int = constrain(ThrottleTPS_0x391_FromGateway_Mapped_Int,0,100);
            //   ThrottleTPS_0x391_FromGateway_Mapped_Int = ThrottleTPS_0x391_FromGateway_Mapped_Int;


            //   AccelPedal_0x391_FromGateway_CAN = Susp_CAN_msg0x391_FromGateway_buf5_Raw;
            //   AccelPedal_0x391_FromGateway_Mapped_Float = (AccelPedal_0x391_FromGateway_CAN * 0.39215686275 ) ;

            //   AccelPedal_0x391_FromGateway_Mapped_Float = map(AccelPedal_0x391_FromGateway_Mapped_Float,15,80,0,100);
            //   AccelPedal_0x391_FromGateway_Mapped_Int = AccelPedal_0x391_FromGateway_Mapped_Float;
            //   AccelPedal_0x391_FromGateway_Mapped_Int = constrain(AccelPedal_0x391_FromGateway_Mapped_Int,0,100);
            //   // AccelPedal_0x391_FromGateway_Mapped_Float = AccelPedal_0x391_FromGateway_Mapped_Int;

            // // Serial.print("ThrottleTPS_0x391: ");
            // // Serial.print(ThrottleTPS_0x391);
            // // Serial.print("AccelPedal_0x391_FromGateway_Mapped: ");
            // // Serial.print(AccelPedal_0x391_FromGateway_Mapped);
            // // Serial.println();

            // // Serial.println();
                
            }

        else if(fromGateway_frame.id == 0x11E){ // Drivetrain/Brakes
        // // printFrame(fromGateway_frame, 3);                                  //uncomment line to print frames received on TFTCAN2
        //   ESP_08_t esp = parse_ESP_08(fromGateway_frame);
        //   MQB_ESP08_0x11E_Slipping_Stillstand = esp.ESP_Slipping_Stillstand;
        //   MQB_ESP08_0x11E_StandStillPhase_Exhausted = esp.ESP_StandStillPhase_Exhausted;
        //   MQB_ESP08_0x11E_VehicleSpeed_ref_Direction_Of_Travel = esp.ESP_v_ref_Direction_Of_Travel;
        //   MQB_ESP08_0x11E_VehicleSpeed_ref = esp.ESP_v_ref;

            }

        else if(fromGateway_frame.id == 0x3C8){ // Gearbox Statuses
          Getriebe_14_t ge14 = parse_Getriebe_14(fromGateway_frame);

          MQB_GE_0x3C8_amax_Possible = ge14.GE_amax_Possible;
          MQB_GE_0x3C8_LaunchControl = ge14.GE_LaunchControl;
          MQB_GE_0x3C8_Sumpftemperatur = ge14.GE_Sumpftemperatur;
          MQB_GE_0x3C8_TorqueLoss = ge14.GE_TorqueLoss;
          MQB_GE_0x3C8_OBD_Status = ge14.GE_OBD_Status;
          
        }

        else if(fromGateway_frame.id == 0xB2) { // Wheel Speed Sensors
        // printFrame(fromGateway_frame, 2);                                  //uncomment line to print frames received on TFTCAN2

            // ESP_19_t esp19 = parse_ESP_19(fromGateway_frame);

            // MQB_ESP_0xB2_HL_Wheel_Speed_02 = esp19.ESP_HL_Wheel_Speed_02;
            // MQB_ESP_0xB2_HR_Wheel_Speed_02 = esp19.ESP_HR_Wheel_Speed_02;
            // MQB_ESP_0xB2_VL_Wheel_Speed_02 = esp19.ESP_VL_Wheel_Speed_02;
            // MQB_ESP_0xB2_VR_Wheel_Speed_02 = esp19.ESP_VR_Wheel_Speed_02;

        }

        else if(fromGateway_frame.id == 0x30B) { // Kombi 

          Kombi_01_t kombi_0x30B = parse_Kombi_01(fromGateway_frame);

        }

        else if(fromGateway_frame.id == 0x3BE) { // Motor_14

           Motor_14_t mo = parse_Motor_14(fromGateway_frame);


        }

        else if(fromGateway_frame.id == 0xAD) { // Kombi 

            Getriebe_11_t g11 = parse_Getriebe_11(fromGateway_frame);


        }

        else if(fromGateway_frame.id == 0x31B) { // ESP_24

        //     ESP_24_t esp24 = parse_ESP_24(fromGateway_frame);


        }









#pragma endregion


  }



  
}


  // Poll TFTCAN3 (PT_CAN Sniffing)//
void loop_TFTCAN3_poll() {
  CAN_message_t PT_CAN_frame;
  CAN_message_t PT_CAN_frame_mod;

    // Simple PT_CAN Sniffing for now
    while (TFTCAN3.read(PT_CAN_frame)) {
 
// Serial.println("tftcan3 recieved");

          if (PT_CAN_frame.id == 0x40) { // Airbag Module - Used for Ign On status
          // printFrame(fromGateway_frame, 3);                                  //uncomment line to print frames received on TFTCAN2
// Serial.println("tftcan3 recieved");

                PT_CAN_msg0x40_buf0_Raw = PT_CAN_frame.buf[0];
                PT_CAN_msg0x40_buf1_Raw = PT_CAN_frame.buf[1];
                PT_CAN_msg0x40_buf2_Raw = PT_CAN_frame.buf[2];
                PT_CAN_msg0x40_buf3_Raw = PT_CAN_frame.buf[3];
                PT_CAN_msg0x40_buf4_Raw = PT_CAN_frame.buf[4];
                PT_CAN_msg0x40_buf5_Raw = PT_CAN_frame.buf[5];
                PT_CAN_msg0x40_buf6_Raw = PT_CAN_frame.buf[6];
                PT_CAN_msg0x40_buf7_Raw = PT_CAN_frame.buf[7];  

                PT_CAN_msg0x40_buf0 = PT_CAN_msg0x40_buf0_Raw;
                PT_CAN_msg0x40_buf1 = PT_CAN_msg0x40_buf1_Raw;
                PT_CAN_msg0x40_buf2 = PT_CAN_msg0x40_buf2_Raw;
                PT_CAN_msg0x40_buf3 = PT_CAN_msg0x40_buf3_Raw;
                PT_CAN_msg0x40_buf4 = PT_CAN_msg0x40_buf4_Raw;
                PT_CAN_msg0x40_buf5 = PT_CAN_msg0x40_buf5_Raw;
                PT_CAN_msg0x40_buf6 = PT_CAN_msg0x40_buf6_Raw;
                PT_CAN_msg0x40_buf7 = PT_CAN_msg0x40_buf7_Raw;  




              // IgnitionStatusTimerCAN(PT_CAN_frame.buf[1]);
              IgnitionStatusTimerCAN = (PT_CAN_msg0x40_buf1_Raw);
              IgnitionStatusTimer = IgnitionStatusTimerCAN;

              IgnitionStatusTimer_TimeSinceLastMessage = 0;

              digitalToggle(LED_PIN_CANRecieve);





              }

          if (PT_CAN_frame.id == 0x120) { // Cruise stalk static position
          // printFrame(fromGateway_frame, 3);                                  //uncomment line to print frames received on TFTCAN2
// Serial.println("tftcan3 recieved");

                PT_CAN_msg0x120_buf0_Raw = PT_CAN_frame.buf[0];
                PT_CAN_msg0x120_buf1_Raw = PT_CAN_frame.buf[1];
                PT_CAN_msg0x120_buf2_Raw = PT_CAN_frame.buf[2];
                PT_CAN_msg0x120_buf3_Raw = PT_CAN_frame.buf[3];
                PT_CAN_msg0x120_buf4_Raw = PT_CAN_frame.buf[4];
                PT_CAN_msg0x120_buf5_Raw = PT_CAN_frame.buf[5];
                PT_CAN_msg0x120_buf6_Raw = PT_CAN_frame.buf[6];
                PT_CAN_msg0x120_buf7_Raw = PT_CAN_frame.buf[7];  

                PT_CAN_msg0x120_buf0 = PT_CAN_msg0x120_buf0_Raw;
                PT_CAN_msg0x120_buf1 = PT_CAN_msg0x120_buf1_Raw;
                PT_CAN_msg0x120_buf2 = PT_CAN_msg0x120_buf2_Raw;
                PT_CAN_msg0x120_buf3 = PT_CAN_msg0x120_buf3_Raw;
                PT_CAN_msg0x120_buf4 = PT_CAN_msg0x120_buf4_Raw;
                PT_CAN_msg0x120_buf5 = PT_CAN_msg0x120_buf5_Raw;
                PT_CAN_msg0x120_buf6 = PT_CAN_msg0x120_buf6_Raw;
                PT_CAN_msg0x120_buf7 = PT_CAN_msg0x120_buf7_Raw;  


                 cruiseStalkStaticPosition_0x120_PTCAN_CAN = PT_CAN_msg0x120_buf3;
                 cruiseStalkStaticPosition_0x120_PT_CAN = cruiseStalkStaticPosition_0x120_PTCAN_CAN;

                if(cruiseStalkStaticPosition_0x120_PT_CAN == 0 ) { // 0x06/ Dec 6 = cruise Stalk in Static ON position, no button pressed
                    CruiseStalk_ON_FromPT_CAN = 0;
                    CruiseStalk_OFF_FromPT_CAN = 1;

                    // LC_BumpIn_ActivationButton = 1;

                    }

                else{ //  = cruise Stalk in Static ON position, no button pressed
                    CruiseStalk_ON_FromPT_CAN = 1;
                    CruiseStalk_OFF_FromPT_CAN = 0;

                    // LC_BumpIn_ActivationButton = 0;

                    }





              }

          if (PT_CAN_frame.id == 0x12B) { // 0x12B - Cruise Stalk Movement


          //  Serial.print("MB "); Serial.print(PT_CAN_frame.mb);
          //  Serial.print(" ID: "); Serial.print(PT_CAN_frame.id, HEX);
          //  Serial.print(" Buffer: ");

          for (uint8_t i = 0; i < PT_CAN_frame.len; i++)
          {
                  //  Serial.print(PT_CAN_frame.buf[i], HEX); Serial.print(" ");
                        // digitalToggle(LED_PIN_CANRecieve);

          }
            //  Serial.println("  ");

          cruiseStalk_byte1CAN = (PT_CAN_frame.buf[1]);
          cruiseStalk_byte1 = cruiseStalk_byte1CAN;

          cruiseStalkCAN = (PT_CAN_frame.buf[2]);
          cruiseStalk = cruiseStalkCAN;





      #pragma region      // Process 0x12b byte 2 - Cruise Stalk Movement Position

        if(cruiseStalk == 128) { // 0x40 or 0x80 =cruise Stalk in Default ON position, no button pressed
            CruiseStalk_ON = 1;
            CruiseStalk_SET = 0;
            CruiseStalk_DOWN = 0;
            CruiseStalk_UP = 0;
            CruiseStalk_RES = 0;
            CruiseStalk_DistanceUP = 0;
            CruiseStalk_DistanceDOWN = 0;
            }

            CruiseStalk_ON_bit = bitRead(cruiseStalk_byte1,4 );


        // if(cruiseStalk == 129 || cruiseStalk == 65) { // 0x41 or 0x81 =cruise SET button pressed
        //     CruiseStalk_SET = 1;
        //     // ConfigurationUpdateMode1_TimeoutTimer = 0;
        //     }
        //   else {
        //     CruiseStalk_SET = 0;
        //       }
            CruiseStalk_SET_bit = bitRead(cruiseStalk, 0);


        // if(cruiseStalk == 130 || cruiseStalk == 66) { // 0x42 or 0x82 =cruise stalk moved UP/towards ceiling
        //     CruiseStalk_UP = 1;
        //     // ConfigurationUpdateMode1_TimeoutTimer = 0;
        //       }
        //   else {
        //     CruiseStalk_UP = 0;
        //       }
            CruiseStalk_UP_bit = bitRead(cruiseStalk, 1);


        // if(cruiseStalk == 132) { // 0x44 or 0x84 =cruise stalk moved DOWN/towards floor
        //     CruiseStalk_DOWN = 1;
        //     // ConfigurationUpdateMode1_TimeoutTimer = 0;
        //     }
        //   else {
        //     CruiseStalk_DOWN = 0;
        //       }
            CruiseStalk_DOWN_bit = bitRead(cruiseStalk, 2);


        // if(cruiseStalk == 136) { // 0x48 or 0x88 =cruise stalk moved Backwards/towards Driver
        //     CruiseStalk_RES = 1;
        //     // ConfigurationUpdateMode1_TimeoutTimer = 0;
        //     }
        //   else {
        //     CruiseStalk_RES = 0;
        //       }
            CruiseStalk_RES_bit = bitRead(cruiseStalk, 3);


        // if(cruiseStalk == 144) { // 0x4 or 0xA0 =cruise stalk up/down distance switch moved DOWN/towards floor
        //     CruiseStalk_DistanceDOWN = 1;
        //     // ConfigurationUpdateMode1_TimeoutTimer = 0;
        //     }
        //   else {
        //     CruiseStalk_DistanceDOWN = 0;
        //       }
            CruiseStalk_DistanceDOWN_bit = bitRead(cruiseStalk, 4);


        // if(cruiseStalk == 160) { // 0x4 or 0xA0 =cruise stalk up/down distance switch moved UP/towards ceiling
        //     CruiseStalk_DistanceUP = 1;
        //     // ConfigurationUpdateMode1_TimeoutTimer = 0;
        //     }
        //   else {
        //     CruiseStalk_DistanceUP = 0;
        //       }
            CruiseStalk_DistanceUP_bit = bitRead(cruiseStalk, 5);


        // if(cruiseStalk_byte1 > 40) { // 0xx or 0xx =cruise stalk moved Forward/towards Engine
        //     CruiseStalk_FORWARD = 1;
        //     // ConfigurationUpdateMode1_TimeoutTimer = 0;
        //     }
        //   else {
        //     CruiseStalk_FORWARD = 0;
        //       }
            CruiseStalk_FORWARD_bit = bitRead(cruiseStalk_byte1, 5);



      #pragma endregion



          }

          if (PT_CAN_frame.id == 0x391){ // Torque Demand/TPS/Accel Pedal 

    //  Serial.print("MB "); Serial.print(PT_CAN_msg0x391.mb);
    //  Serial.print(" ID: "); Serial.print(PT_CAN_msg0x391.id, HEX);
    //  Serial.print(" Buffer: ");
    for (uint8_t i = 0; i < PT_CAN_frame.len; i++)
    {
        //  Serial.print(PT_CAN_msg0x391.buf[i], HEX); Serial.print(" ");
    }
          // Serial.println("  ");





                PT_CAN_msg0x391_buf0_Raw = PT_CAN_frame.buf[0];
                PT_CAN_msg0x391_buf1_Raw = PT_CAN_frame.buf[1];
                PT_CAN_msg0x391_buf2_Raw = PT_CAN_frame.buf[2];
                PT_CAN_msg0x391_buf3_Raw = PT_CAN_frame.buf[3];
                PT_CAN_msg0x391_buf4_Raw = PT_CAN_frame.buf[4];
                PT_CAN_msg0x391_buf5_Raw = PT_CAN_frame.buf[5];
                PT_CAN_msg0x391_buf6_Raw = PT_CAN_frame.buf[6];
                PT_CAN_msg0x391_buf7_Raw = PT_CAN_frame.buf[7];  

                PT_CAN_msg0x391_buf0 = PT_CAN_msg0x391_buf0_Raw;
                PT_CAN_msg0x391_buf1 = PT_CAN_msg0x391_buf1_Raw;
                PT_CAN_msg0x391_buf2 = PT_CAN_msg0x391_buf2_Raw;
                PT_CAN_msg0x391_buf3 = PT_CAN_msg0x391_buf3_Raw;
                PT_CAN_msg0x391_buf4 = PT_CAN_msg0x391_buf4_Raw;
                PT_CAN_msg0x391_buf5 = PT_CAN_msg0x391_buf5_Raw;
                PT_CAN_msg0x391_buf6 = PT_CAN_msg0x391_buf6_Raw;
                PT_CAN_msg0x391_buf7 = PT_CAN_msg0x391_buf7_Raw;  



              ThrottleTPS_0x391_PTCAN_CAN = PT_CAN_msg0x391_buf2;
              ThrottleTPS_0x391_PT_CAN_Float = (ThrottleTPS_0x391_PTCAN_CAN * 0.39215686275 ) ;

              ThrottleTPS_0x391_PT_CAN_Mapped_Float = map(ThrottleTPS_0x391_PT_CAN_Float,10,85,0,100);
              ThrottleTPS_0x391_PT_CAN_Mapped_Int = ThrottleTPS_0x391_PT_CAN_Mapped_Float;
              ThrottleTPS_0x391_PT_CAN_Mapped_Int = constrain(ThrottleTPS_0x391_PT_CAN_Mapped_Int,0,100);
              ThrottleTPS_0x391_PT_CAN_Mapped_Int = ThrottleTPS_0x391_PT_CAN_Mapped_Int;


              AccelPedal_0x391_PT_CAN = PT_CAN_msg0x391_buf5;
              AccelPedal_0x391_PT_CAN_Float = (AccelPedal_0x391_PT_CAN * 0.39215686275 ) ;

              AccelPedal_0x391_PT_CAN_Mapped_Float = map(AccelPedal_0x391_PT_CAN_Float,15,80,0,100);
              AccelPedal_0x391_PT_CAN_Mapped_Int = AccelPedal_0x391_PT_CAN_Mapped_Float;
              AccelPedal_0x391_PT_CAN_Mapped_Int = constrain(AccelPedal_0x391_PT_CAN_Mapped_Int,0,100);

            // Serial.print("ThrottleTPS_0x391_PT_CAN_Float: ");
            // Serial.print(ThrottleTPS_0x391_PT_CAN_Float); Serial.print(" ");
            // Serial.print("AccelPedal_0x391_PT_CAN_Mapped_Float: ");
            // Serial.println(AccelPedal_0x391_PT_CAN_Mapped_Float);  Serial.print(" ");
            // Serial.println();

            // Serial.println();
                
            }

          if (PT_CAN_frame.id == 0x3DD){ // paddles
// Serial.println("tftcan3 recieved");


                 PT_CAN_msg0x3DD_buf0_Raw = PT_CAN_frame.buf[0];
                 PT_CAN_msg0x3DD_buf1_Raw = PT_CAN_frame.buf[1];
                 PT_CAN_msg0x3DD_buf2_Raw = PT_CAN_frame.buf[2];
                 PT_CAN_msg0x3DD_buf3_Raw = PT_CAN_frame.buf[3];
                 PT_CAN_msg0x3DD_buf4_Raw = PT_CAN_frame.buf[4];
                 PT_CAN_msg0x3DD_buf5_Raw = PT_CAN_frame.buf[5];
                 PT_CAN_msg0x3DD_buf6_Raw = PT_CAN_frame.buf[6];
                 PT_CAN_msg0x3DD_buf7_Raw = PT_CAN_frame.buf[7];

                 PT_CAN_msg0x3DD_buf0 = PT_CAN_msg0x3DD_buf0_Raw;
                 PT_CAN_msg0x3DD_buf1 = PT_CAN_msg0x3DD_buf1_Raw;
                 PT_CAN_msg0x3DD_buf2 = PT_CAN_msg0x3DD_buf2_Raw;
                 PT_CAN_msg0x3DD_buf3 = PT_CAN_msg0x3DD_buf3_Raw;
                 PT_CAN_msg0x3DD_buf4 = PT_CAN_msg0x3DD_buf4_Raw;
                 PT_CAN_msg0x3DD_buf5 = PT_CAN_msg0x3DD_buf5_Raw;
                 PT_CAN_msg0x3DD_buf6 = PT_CAN_msg0x3DD_buf6_Raw;
                 PT_CAN_msg0x3DD_buf7 = PT_CAN_msg0x3DD_buf7_Raw;



                      word PaddlePositionsCAN = PT_CAN_msg0x3DD_buf7;
                      PaddlePositions = PaddlePositionsCAN;

      if(PaddlePositions == 0x00)
        {
        Paddle_DOWN_Pulled = 0;
        Paddle_UP_Pulled = 0;
        BOTH_Paddles_Pulled = 0;
        }

      if(PaddlePositions == 0x01)
        {
        Paddle_DOWN_Pulled = 1;
        Paddle_UP_Pulled = 0;
        BOTH_Paddles_Pulled = 0;
        }

      if(PaddlePositions == 0x02)
        {
        
        Paddle_DOWN_Pulled = 0;
        Paddle_UP_Pulled = 1;
        BOTH_Paddles_Pulled = 0;
        }

      if(PaddlePositions == 0x03)
        {
        Paddle_DOWN_Pulled = 0;
        Paddle_UP_Pulled = 0;
        BOTH_Paddles_Pulled = 1;
        }

                                  
            }

            


      // // Determine Launch Control Bump-in Activation BUTTON
      //      if(( CruiseStalk_OFF_FromPT_CAN == 1 || Paddle_DOWN_Pulled == 1 ) && WhlSpdRR < 10 ) { // 

      //         LC_BumpIn_ActivationButton = 1;

      //         }

      //      else{ // 

      //         LC_BumpIn_ActivationButton = 0;

      //         }





      // // Send Operational Statuses periodically, for TFT Module and ECU to recognize MITM Statuses

      // if(LC_BumpIn_AcceptableConditions == 1) {


      //         CAN_message_t PT_CAN_msg0x795;
      //         PT_CAN_msg0x795.len = 8;
      //         PT_CAN_msg0x795.id = 0x795;
      //         PT_CAN_msg0x795.buf[0] = lowByte(LC_BumpIn_AcceptableConditions);
      //         PT_CAN_msg0x795.buf[1] = 00;
      //         PT_CAN_msg0x795.buf[2] = lowByte(TFT_MITM_LC_BumpIn_Status_Active_canTx);
      //         PT_CAN_msg0x795.buf[3] = 00;
      //         PT_CAN_msg0x795.buf[4] = lowByte(TFT_MITM_LC_BumpInActive_RollingStatus_canTx);
      //         PT_CAN_msg0x795.buf[5] = 00;
      //         PT_CAN_msg0x795.buf[6] = lowByte(TFT_MITM_LC_BumpIn_TimeSinceActiveMS_canTx_16bit);
      //         PT_CAN_msg0x795.buf[7] = highByte(TFT_MITM_LC_BumpIn_TimeSinceActiveMS_canTx_16bit);




      //     if (PT_CAN_canTxInterval_0x795 > 200) { // this is in millis
      //           TFTCAN3.write(PT_CAN_msg0x795); //  CAN Frame for Yaw, Pitch, Roll
      //           PT_CAN_canTxInterval_0x795 = 0;
      //         }



  
  
      //       }



  }


  
}



void do_TFT_MITM_LC_BumpIn_Statuses() {



        if(WhlSpdRR_fromABS_Int < 5) { // Initial Determination of "Acceptable Conditions" for LC BumpIn Activation

              LC_BumpIn_AcceptableConditions = 1;
              TFT_MITM_LC_BumpIn_Status_Armed_Int = 1;
          }

          else {
                LC_BumpIn_AcceptableConditions = 0;
                TFT_MITM_LC_BumpIn_Status_Armed_Int = 0;
            }


      // Determine Launch Control Bump-in Activation BUTTON
          if(( CruiseStalk_OFF_FromPT_CAN == 1 || Paddle_DOWN_Pulled == 1 ) && LC_BumpIn_AcceptableConditions == 1 ) { // 

              LC_BumpIn_ActivationButton = 1;

              }

            else{ // 

                    LC_BumpIn_ActivationButton = 0;
                    TFT_MITM_LC_BumpIn_TimeSinceActiveMS_canTx_16bit = 0;

                    }


      // If Launch Control Bump-In Status is Active, then Update canTx variables
      if(LC_BumpIn_ActivationStatus == 1) {
          TFT_MITM_LC_BumpIn_TimeSinceActiveMS_canTx_16bit = LC_BumpIn_TimeSinceActivation;
          TFT_MITM_LC_BumpIn_Status_Active_Int = 1;
          }

        else{
            TFT_MITM_LC_BumpIn_TimeSinceActiveMS_canTx_16bit = 0;
            TFT_MITM_LC_BumpIn_Status_Active_Int = 0;
            }


      // Determine Rolling Status if LC Bump-In Status is active and the ABS module is showing true wheel speed > 0
      if(LC_BumpIn_ActivationStatus == 1 && WhlSpdRR_fromABS_Float_Raw > 0.5 ) {
          TFT_MITM_LC_BumpInActive_RollingStatus_Int = 1;
          // TrueWhlSpdRR_fromABS_canTx_16bit = WhlSpdRR_fromABS_Float_Raw * 10;
      }  
      else {
          TFT_MITM_LC_BumpInActive_RollingStatus_Int = 0;
      }


      // Send Operational Statuses periodically, for TFT Module and ECU to recognize MITM Statuses

      // if(LC_BumpIn_AcceptableConditions == 0) {
      if(LC_BumpIn_AcceptableConditions < 5) {

            TFT_MITM_LC_BumpIn_Status_Combined_Int = ((LC_BumpIn_AcceptableConditions * 1) + (TFT_MITM_LC_BumpIn_Status_Active_Int * 1) + (TFT_MITM_LC_BumpInActive_RollingStatus_Int * 2));

              CAN_message_t PT_CAN_msg0x795;
              PT_CAN_msg0x795.len = 8;
              PT_CAN_msg0x795.id = 0x795;
              PT_CAN_msg0x795.buf[0] = 0x3;
              PT_CAN_msg0x795.buf[1] =  lowByte(TFT_MITM_LC_BumpIn_Status_Combined_Int);
              PT_CAN_msg0x795.buf[2] =  lowByte(TFT_MITM_LC_BumpIn_TimeSinceActiveMS_canTx_16bit);
              PT_CAN_msg0x795.buf[3] = highByte(TFT_MITM_LC_BumpIn_TimeSinceActiveMS_canTx_16bit);
              PT_CAN_msg0x795.buf[4] =  lowByte(TFT_MITM_LC_BumpIn_TrueWhlSpdRR_fromABS_canTx_16bit);
              PT_CAN_msg0x795.buf[5] = highByte(TFT_MITM_LC_BumpIn_TrueWhlSpdRR_fromABS_canTx_16bit);
              PT_CAN_msg0x795.buf[6] =  lowByte(TFT_MITM_LC_BumpIn_TrueBrakePressureMPA_fromABS_canTx_16bit);
              PT_CAN_msg0x795.buf[7] = highByte(TFT_MITM_LC_BumpIn_TrueBrakePressureMPA_fromABS_canTx_16bit);




          if (PT_CAN_canTxInterval_0x795 > 30) { // this is in millis
                TFTCAN3.write(PT_CAN_msg0x795); //  CAN Frame for Yaw, Pitch, Roll
                PT_CAN_canTxInterval_0x795 = 0;
              }

  
  
        }


}




void loop_SerialPrinting_MITM(){

    if(loopDelaySerialPrint > 100) {
                  //  Serial.println("");

            Serial.print("lsbInternalCPUTemp: ");  Serial.print(lsbInternalCPUTemp);  Serial.print("\t ");

            // Serial.print("ThrottleTPS_0x391_FromGateway_Mapped_Float: ");  Serial.print(ThrottleTPS_0x391_FromGateway_Mapped_Float);  Serial.print("\t ");
            // Serial.print("ThrottleTPS_0x391_FromGatewayCAN: ");  Serial.print(ThrottleTPS_0x391_FromGatewayCAN);  Serial.print("\t ");
            // Serial.print("AccelPedal_0x391_FromGateway_CAN: ");  Serial.print(AccelPedal_0x391_FromGateway_CAN);  Serial.print("\t ");
            // Serial.print("AccelPedal_0x391_FromGateway_Mapped: ");  Serial.print(AccelPedal_0x391_FromGateway_Mapped);  Serial.print("\t ");
            // Serial.print("AccelPedal_0x391_FromGateway_MapF: ");  Serial.print(AccelPedal_0x391_FromGateway_Mapped_Float);  Serial.print("\t ");
            // Serial.print("BrakePedalSwitchStatus: ");  Serial.print(BrakePedalSwitchStatus);  Serial.print("\t ");
            // Serial.print("BrakeSwitch_0x1AB_CAN: ");  Serial.print(BrakeSwitch_0x1AB_CAN);  Serial.print("\t");
            // Serial.print("ParkBrakeStatus_0x104_FromGateway: ");  Serial.print(ParkBrakeStatus_0x104_FromGateway);  Serial.print("\t");
            // Serial.print("CruiseStalk_ON_FromGateway: ");  Serial.print(CruiseStalk_ON_FromGateway);  Serial.print("\t");
            // Serial.print("CruiseStalk_OFF_FromGateway: ");  Serial.print(CruiseStalk_OFF_FromGateway);  Serial.print("\t");
            // Serial.print("BrakePedalSwitchStatus_FromGateway: ");  Serial.print(BrakePedalSwitchStatus_FromGateway);  Serial.print("\t"); //Good
            // Serial.print("BrakePressureBAR_FromGateway_Float: ");  Serial.print(BrakePressureBAR_FromGateway_Float);  Serial.print("\t"); // Good
            // Serial.print("ThrottleTPS_0x391_FromABS_Mapped_Float: ");  Serial.print(ThrottleTPS_0x391_FromABS_Mapped_Float);  Serial.print("\t");
            // Serial.print("ThrottleTPS_0x391_FromABSCAN: ");  Serial.print(ThrottleTPS_0x391_FromABSCAN);  Serial.print("\t");
            // Serial.print("AccelPedal_0x391_FromABS_CAN: ");  Serial.print(AccelPedal_0x391_FromABS_CAN);  Serial.print("\t");
            // Serial.print("AccelPedal_0x391_FromABS_Mapped: ");  Serial.print(AccelPedal_0x391_FromABS_Mapped);  Serial.print("\t");
            // Serial.print("AccelPedal_0x391_FromABS_Mapped_Float: ");  Serial.print(AccelPedal_0x391_FromABS_Mapped_Float);  Serial.print("\t");
            // Serial.print("BrakePedalSwitchStatus: ");  Serial.print(BrakePedalSwitchStatus);  Serial.print("\t");
            // Serial.print("BrakeSwitch_0x1AB_FromABS: ");  Serial.print(BrakeSwitch_0x1AB_FromABS);  Serial.print("\t");
            // Serial.print("ParkBrakeStatus_0x104_FromABS: ");  Serial.print(ParkBrakeStatus_0x104_FromABS);  Serial.print("\t");
            // Serial.print("CruiseStalk_ON_FromABS: ");  Serial.print(CruiseStalk_ON_FromABS);  Serial.print("\t");
            // Serial.print("CruiseStalk_OFF_FromABS: ");  Serial.print(CruiseStalk_OFF_FromABS);  Serial.print("\t");
            // Serial.print("BrakePedalSwitchStatus_FromABS: ");  Serial.print(BrakePedalSwitchStatus_FromABS);  Serial.print("\t"); //Good
          //  Serial.print("LC_BumpIn_ActvButt: ");  Serial.print(LC_BumpIn_ActivationButton);  Serial.print("\t ");
            Serial.print("LC_BumpIn_ActvStat: ");  Serial.print(LC_BumpIn_ActivationStatus);  Serial.print("\t ");
            // Serial.print("CCStalk_0x120_FromGateway: ");  Serial.print(cruiseStalkStaticPosition_0x120_FromGateway);  Serial.print("\t ");
            // Serial.print("cruiseStalkStaticPosition_0x120_PT_CAN: ");  Serial.print(cruiseStalkStaticPosition_0x120_PT_CAN);  Serial.print("\t ");

            
            Serial.print("MITM_Status_Combined_Int: ");  Serial.print(TFT_MITM_LC_BumpIn_Status_Combined_Int);  Serial.print("\t ");
            Serial.print("MITM_TimeSinceActiveMS_16bit: ");  Serial.print(TFT_MITM_LC_BumpIn_TimeSinceActiveMS_canTx_16bit);  Serial.print("\t ");
            Serial.print("MITM_Status_TrueWhlSpdRR_16bit: ");  Serial.print(TFT_MITM_LC_BumpIn_TrueWhlSpdRR_fromABS_canTx_16bit);  Serial.print("\t ");
            Serial.print("MITM_Status_TrueBrkPresMPA_16bit: ");  Serial.print(TFT_MITM_LC_BumpIn_TrueBrakePressureMPA_fromABS_canTx_16bit);  Serial.print("\t ");




            // Serial.print("LC_BumpIn_TimeSinceActivation: ");  Serial.print(LC_BumpIn_TimeSinceActivation);  Serial.print("\t ");
            // Serial.print("BrakePedalSwitchStatus: ");  Serial.print(BrakePedalSwitchStatus);  Serial.print("\t");
            // Serial.print("BrakePressureBAR_atTimeof_LCBumpInActivation_Float: ");  Serial.print(BrakePressureBAR_atTimeof_LCBumpInActivation_Float);  Serial.print("\t");
            // Serial.print("BrakePressureBAR_atTimeobuf2: ");  Serial.print(BrakePressureBAR_atTimeof_LCBumpInActivation_Raw_buf2);  Serial.print("\t");
            // Serial.print("BrakePressureBAR_atTimeobuf3: ");  Serial.print(BrakePressureBAR_atTimeof_LCBumpInActivation_Raw_buf3);  Serial.print("\t");
            // Serial.print("BrakePressureBAR_0x106_Raw: ");  Serial.print(BrakePressureBAR_0x106_Raw);  Serial.print("\t");
            // Serial.print("BrakePressureBAR_Float: ");  Serial.print(BrakePressureBAR_Float);  Serial.print("\t");
            // Serial.print("BrakePressureBAR_Int: ");  Serial.print(BrakePressureBAR_Int);  Serial.print("\t");
            // Serial.print("WhlSpdRL: ");  Serial.print(WhlSpdRL);  Serial.print("\t");
            // Serial.print("WhlSpdRL_Raw_CAN: ");  Serial.print(WhlSpdRL_Raw_CAN);  Serial.print("\t");
            // Serial.print("WhlSpdRR: ");  Serial.print(WhlSpdRR);  Serial.print("\t");
            // Serial.print("WhlSpdRR_fromABS_FltRaw: ");  Serial.print(WhlSpdRR_fromABS_Float_Raw);  Serial.print("\t");
            // Serial.print("WhlSpdRR_Raw_CAN: ");  Serial.print(WhlSpdRR_Raw_CAN);  Serial.print("\t");
            // Serial.print("TrueWhlSpdRRFlt: ");  Serial.print(TrueWhlSpdRR_fromABS_canTx_16bit);  Serial.print("\t");

            // Serial.print("WhlSpdFL: ");  Serial.print(WhlSpdFL);  Serial.print("\t");
            // Serial.print("WhlSpdFL_Raw_CAN: ");  Serial.print(WhlSpdFL_Raw_CAN);  Serial.print("\t");
            // Serial.print("WhlSpdFR: ");  Serial.print(WhlSpdFR);  Serial.print("\t");
            // Serial.print("WhlSpdFR_Raw_CAN: ");  Serial.print(WhlSpdFR_Raw_CAN);  Serial.print("\t");

            // BrakePedalSwitchStatus_FromABS

            // Serial.print("APP_0x391_PT_CAN_MapF: ");  Serial.print(AccelPedal_0x391_PT_CAN_Mapped_Float);  Serial.print("\t ");
            // Serial.print("CruiseStalk_ON_FromPT_CAN: ");  Serial.print(CruiseStalk_ON_FromPT_CAN);  Serial.print("\t ");
            // Serial.print("cruiseStalkStaticPos_0x120PTCAN: ");  Serial.print(cruiseStalkStaticPosition_0x120_PTCAN_CAN);  Serial.print("\t ");
            // Serial.print("CC_OFF_FromPT_CAN: ");  Serial.print(CruiseStalk_OFF_FromPT_CAN);  Serial.print("\t ");
            // Serial.print("Paddle_DOWN_: ");  Serial.print(Paddle_DOWN_Pulled);  Serial.print("\t ");
            // Serial.print("PaddlePositions: ");  Serial.print(PaddlePositions);  Serial.print("\t ");



        #pragma region // DBC Parsed

        // ESP - Brakes and Chassis Movement
            // Serial.print("MQB_ESP05_0x106_BrakePressureBAR_Float: ");  Serial.print(MQB_ESP05_0x106_BrakePressureBAR_Float);  Serial.print("\t"); // Good
            Serial.print("MQB_ESP05_0x106_BrkPrsMPA: ");  Serial.print(MQB_ESP05_0x106_BrakePressureMPA_Float);  Serial.print("\t"); // Good
            // Serial.print("MQB_ESP05_0x106_Brake_Pressure: ");  Serial.print(MQB_ESP05_0x106_Brake_Pressure);  Serial.print("\t"); // Good
            // Serial.print("MQB_ESP05_0x106_Driver_Brakes: ");  Serial.print(MQB_ESP05_0x106_Driver_Brakes);  Serial.print("\t"); // Good
            // Serial.print("MQB_ESP05_0x106_Status_Brake_Pressure: ");  Serial.print(MQB_ESP05_0x106_Status_Brake_Pressure);  Serial.print("\t"); // Good
            // Serial.print("MQB_ESP05_0x106_QBit_Driver_Brakes: ");  Serial.print(MQB_ESP05_0x106_QBit_Driver_Brakes);  Serial.print("\t"); // Good
            // Serial.print("MQB_ESP05_0x106_Status_Brake_Pressure: ");  Serial.print(MQB_ESP05_0x106_Status_Brake_Pressure);  Serial.print("\t"); // Good
            // Serial.print("MQB_ESP05_0x106_Threshold_Negative_Pressure: ");  Serial.print(MQB_ESP05_0x106_Threshold_Negative_Pressure);  Serial.print("\t"); // Good
            // Serial.print("MQB_ECD_0x106_Brake_Light: ");  Serial.print(MQB_ECD_0x106_Brake_Light);  Serial.print("\t"); // not good
            // Serial.print("MQB_ESP05_0x106_Eingr_HL: ");  Serial.print(MQB_ESP05_0x106_Eingr_HL);  Serial.print("\t"); // Good
            // Serial.print("MQB_ESP05_0x106_Eingr_HR: ");  Serial.print(MQB_ESP05_0x106_Eingr_HR);  Serial.print("\t"); // Good
            // Serial.print("MQB_ESP05_0x106_Eingr_VL: ");  Serial.print(MQB_ESP05_0x106_Eingr_VL);  Serial.print("\t"); // Good
            // Serial.print("MQB_ESP05_0x106_Eingr_VR: ");  Serial.print(MQB_ESP05_0x106_Eingr_VR);  Serial.print("\t"); // Good
            

            // Serial.print("MQB_ESP_0x101_Standstill_Flag: ");  Serial.print(MQB_ESP_0x101_Standstill_Flag);  Serial.print("\t"); // Good
            // Serial.print("MQB_ESP_0x101_QBit_LongitudinalMovement: ");  Serial.print(MQB_ESP_0x101_QBit_LongitudinalMovement);  Serial.print("\t"); // Good
            // Serial.print("MQB_ESP_0x101_QBit_LateralMovement: ");  Serial.print(MQB_ESP_0x101_QBit_LateralMovement);  Serial.print("\t"); // Good
            // Serial.print("MQB_ESP_0x101_LongitudinalAcceleration: ");  Serial.print(MQB_ESP_0x101_LongitudinalAcceleration);  Serial.print(" \t "); // Good
            // Serial.print("MQB_ESP_0x101_LateralAcceleration: ");  Serial.print(MQB_ESP_0x101_LateralAcceleration);  Serial.print(" \t "); // Good
            // Serial.print("MQB_ESP_0x101_Yaw_Rate: ");  Serial.print(MQB_ESP_0x101_Yaw_Rate);  Serial.print(" \t "); // Good
            // Serial.print("MQB_ESP_0x101_PLA_Abort: ");  Serial.print(MQB_ESP_0x101_PLA_Abort);  Serial.print(" \t "); // Good
            // Serial.print("MQB_ESP_0x101_Status_ESP_PLA: ");  Serial.print(MQB_ESP_0x101_Status_ESP_PLA);  Serial.print(" \t "); // Good
            // Serial.print("MQB_ESP_0x101_QBit_Initial_Value_Wank: ");  Serial.print(MQB_ESP_0x101_QBit_Initial_Value_Wank);  Serial.print(" \t "); // Good
            // Serial.print("MQB_ESP_0x101_DistributionWankmom: ");  Serial.print(MQB_ESP_0x101_Distribution_Wankmom);  Serial.print(" \t "); // Good


            // Serial.print("MQB_ESP08_0x11E_Slipping_Stillstand: ");  Serial.print(MQB_ESP08_0x11E_Slipping_Stillstand);  Serial.print(" \t "); // Good
            // Serial.print("MQB_ESP08_0x11E_StandStillPhase_Exhausted: ");  Serial.print(MQB_ESP08_0x11E_StandStillPhase_Exhausted);  Serial.print(" \t "); // Good
            // Serial.print("MQB_ESC_0x11E_Brake_Pressure_Gradient: ");  Serial.print(MQB_ESC_0x11E_Brake_Pressure_Gradient);  Serial.print(" \t "); // Good
            // Serial.print("MQB_ESP08_0x11E_VehicleSpeed_ref_Direction_Of_Travel: ");  Serial.print(MQB_ESP08_0x11E_VehicleSpeed_ref_Direction_Of_Travel);  Serial.print(" \t "); // Good
            // Serial.print("MQB_ESP08_0x11E_VehicleSpeed_ref: ");  Serial.print(MQB_ESP08_0x11E_VehicleSpeed_ref);  Serial.print(" \t "); // Good

            // Serial.print("MQB_ESP10_0x116_HL_Direction_of_Travel: ");  Serial.print(MQB_ESP10_0x116_HL_Direction_of_Travel);  Serial.print(" \t "); // Good
            // Serial.print("MQB_ESP10_0x116_HR_Direction_of_Travel: ");  Serial.print(MQB_ESP10_0x116_HR_Direction_of_Travel);  Serial.print(" \t "); // Good
            // Serial.print("MQB_ESP10_0x116_VL_Direction_of_Travel: ");  Serial.print(MQB_ESP10_0x116_VL_Direction_of_Travel);  Serial.print(" \t "); // Good
            // Serial.print("MQB_ESP10_0x116_VR_Direction_of_Travel: ");  Serial.print(MQB_ESP10_0x116_VR_Direction_of_Travel);  Serial.print(" \t "); // Good
            // Serial.print("MQB_ESP10_0x116_Path_Impulse_HL: ");  Serial.print(MQB_ESP10_0x116_Path_Impulse_HL);  Serial.print(" \t "); // Good
            // Serial.print("MQB_ESP10_0x116_Path_Impulse_HR: ");  Serial.print(MQB_ESP10_0x116_Path_Impulse_HR);  Serial.print(" \t "); // Good
            // Serial.print("MQB_ESP10_0x116_Path_Impulse_VL: ");  Serial.print(MQB_ESP10_0x116_Path_Impulse_VL);  Serial.print(" \t "); // Good
            // Serial.print("MQB_ESP10_0x116_Path_Impulse_VR: ");  Serial.print(MQB_ESP10_0x116_Path_Impulse_VR);  Serial.print(" \t "); // Good
           
           
            // Serial.print("MQB_GE_0x3C8_amax_Possible: ");  Serial.print(MQB_GE_0x3C8_amax_Possible);  Serial.print(" \t "); // Good
            // Serial.print("MQB_GE_0x3C8_LaunchControl: ");  Serial.print(MQB_GE_0x3C8_LaunchControl);  Serial.print(" \t "); // Good
            // Serial.print("MQB_GE_0x3C8_Sumpftemperatur: ");  Serial.print(MQB_GE_0x3C8_Sumpftemperatur);  Serial.print(" \t "); // Good
            // Serial.print("MQB_GE_0x3C8_TorqueLoss: ");  Serial.print(MQB_GE_0x3C8_TorqueLoss);  Serial.print(" \t "); // Good
            // Serial.print("MQB_GE_0x3C8_OBD_Status: ");  Serial.print(MQB_GE_0x3C8_OBD_Status);  Serial.print(" \t "); // Good

            // Serial.print("MQB_ESP07_0x392_RTA_HL: ");  Serial.print(MQB_ESP07_0x392_RTA_HL);  Serial.print(" \t "); // Good
            // Serial.print("MQB_ESP07_0x392_RTA_HR: ");  Serial.print(MQB_ESP07_0x392_RTA_HR);  Serial.print(" \t "); // Good
            // Serial.print("MQB_ESP07_0x392_RTA_VR: ");  Serial.print(MQB_ESP07_0x392_RTA_VR);  Serial.print(" \t "); // Good

            // Serial.print("MQB_OBD_0x392_Wheel_Sensor_Error_HL: ");  Serial.print(MQB_OBD_0x392_Wheel_Sensor_Error_HL);  Serial.print(" \t "); // Good
            // Serial.print("MQB_OBD_0x392_Wheel_Sensor_Error_HR: ");  Serial.print(MQB_OBD_0x392_Wheel_Sensor_Error_HR);  Serial.print(" \t "); // Good
            // Serial.print("MQB_OBD_0x392_Wheel_Sensor_Error_VL: ");  Serial.print(MQB_OBD_0x392_Wheel_Sensor_Error_VL);  Serial.print(" \t "); // Good
            // Serial.print("MQB_OBD_0x392_Wheel_Sensor_Error_VR: ");  Serial.print(MQB_OBD_0x392_Wheel_Sensor_Error_VR);  Serial.print(" \t "); // Good


          
            // Serial.print("MQB_ESC_0xFD_v_Signal_Qualifier_High_Low: ");  Serial.print(MQB_ESC_0xFD_v_Signal_Qualifier_High_Low);  Serial.print("\t"); // Good
            // Serial.print("MQB_BR_0xFD_Engagement_Torque: ");  Serial.print(MQB_BR_0xFD_Engagement_Torque);  Serial.print("\t"); // Good
            // Serial.print("MQB_ABS_0xFD_Braking: ");  Serial.print(MQB_ABS_0xFD_Braking);  Serial.print("\t"); // Good
            // Serial.print("MQB_ASR_0xFD_Intervention_Switch: ");  Serial.print(MQB_ASR_0xFD_Intervention_Switch);  Serial.print("\t"); // Good
            // Serial.print("MQB_ESC_0xFD_Reku_Release: ");  Serial.print(MQB_ESC_0xFD_Reku_Release);  Serial.print("\t"); // Good
            // Serial.print("MQB_ESP21_0xFD_ASP: ");  Serial.print(MQB_ESP21_0xFD_ASP);  Serial.print("\t"); // Good
            // Serial.print("MQB_ESP21_0xFD_QBit_v_Signal: ");  Serial.print(MQB_ESP21_0xFD_QBit_v_Signal);  Serial.print("\t"); // Good
            // Serial.print("MQB_ESP21_0xFD_VehicleSpeed_Signal: ");  Serial.print(MQB_ESP21_0xFD_VehicleSpeed_Signal);  Serial.print("\t"); // Good
            
            
            // Serial.print("MQB_ESP_0xB2_HL_Wheel_Speed_02: ");  Serial.print(MQB_ESP_0xB2_HL_Wheel_Speed_02);  Serial.print("\t"); // Good
            // Serial.print("MQB_ESP_0xB2_HR_Wheel_Speed_02: ");  Serial.print(MQB_ESP_0xB2_HR_Wheel_Speed_02);  Serial.print("\t"); // Good
            // Serial.print("MQB_ESP_0xB2_VL_Wheel_Speed_02: ");  Serial.print(MQB_ESP_0xB2_VL_Wheel_Speed_02);  Serial.print("\t"); // Good
            // Serial.print("MQB_ESP_0xB2_VR_Wheel_Speed_02: ");  Serial.print(MQB_ESP_0xB2_VR_Wheel_Speed_02);  Serial.print("\t"); // Good



            #pragma endregion


            Serial.println("");

            loopDelaySerialPrint = 0;
        }



  }