////
#include <global_vars.h>
#include <TFTCAN_PT_CAN.h>
#include <FlexCAN_T4.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <elapsedMillis.h>
#include <IMU.h>
#include <TFTCAN_PT_CAN.h>
#include <Configurations_Resets_wDisplayGauges.h>
// #include "Configurations_Resets_wDisplayGauges.h"

#ifdef Concept_TFT_StandaloneHaldexController
#include <TFTCAN_SUSP_CAN.h>
#endif

// #define MQB_AutoUPshift_Gear1to2_RPMSetpoint 1800
// #define MQB_AutoUPshift_Gear2to3_RPMSetpoint 2000
// #define MQB_AutoUPshift_Gear3to4_RPMSetpoint 2500
// #define MQB_AutoUPshift_Gear4to5_RPMSetpoint 2500
// #define MQB_AutoUPshift_Gear5to6_RPMSetpoint 2500
// #define MQB_AutoUPshift_Gear6to7_RPMSetpoint 2500


#pragma region // Reference - VAG/MQB related Module Addresses

  // Potential VAG Module addresses.
  // 0x080    Motor_01	
  // 0x081    Motor_02	
  // 0x082    Getriebe_01	
  // 0x083    Getriebe_02	
  // 0x086    LWI_01		
  // 0x0AA    Motor_13	
  // 0xAB    Getriebe_07	
  // 0x0B0    			
  // 0x100    ESP_01
  // 0x101    ESP_02	
  // 0x102    Getriebe_03	
  // 0x103    ESP_03		
  // 0x104    EPB_01		
  // 0x105    Motor_03	
  // 0x106    ESP_05		
  // 0x107    Motor_04	
  // 0x10B    LS_01		
  // 0x10E    TSK_04
  // 0x111    TSK_05
  // 0x114    Motor_10	
  // 0x133    			
  // 0x308    			
  // 0x309    BEM_01		
  // 0x30B    Kombi_01	
  // 0x30E    Motor_05	
  // 0x312    TSK_03
  // 0x383    			
  // 0x384    			
  // 0x385    Charisma_01	
  // 0x38A    GW_FWK_01_Ant_PAG	
  // 0x391    OBD_01	Y		
  // 0x392    ESP_07_FR		
  // 0x393    WBA_02	N/A	
  // 0x39C    Gateway_05		
  // 0x3A5    Motor_15_PAG	
  // 0x3B2    BEM_03			
  // 0x3BF    Klima_01	
  // 0x3C0    Klemmen_Status_01	
  // 0x440    	Motor_06	
  // 0x441    	Getriebe_04	
  // 0x471    	Licht_hinten_01		
  // 0x520    	Airbag_02	
  // 0x523    				
  // 0x585    	Systeminfo_01	
  // 0x590    	Kessy_02	
  // 0x5EE    				
  // 0x5F0    	Dimmung_01			
  // 0x640    	Motor_07	
  // 0x641    	Motor_Code_01 
  // 0x642    	SRV_INTL_1	
  // 0x644    	Gateway_11		
  // 0x647    	Motor_09	
  // 0x64C    				
  // 0x6AA    				
  // 0x6AC    		
  // 0x6B2    	Diagnose_01			Y
  // 0x6B4    	VIN_01			
  // 0x6B7    	Kombi_02		Y	

#pragma endregion

#pragma region // Definitions - ON-Board LEDs and functions

  // #define LED_PIN_CANTransmit 49
  // bool blinkStateCANTransmit = false;

  // #define LED_PIN_CANRecieve 54
  // bool blinkStateCANRecieve = false;

  // #define LED_PIN_CANDelay 50
  // bool blinkStateCANDelay = false;


#pragma endregion

#pragma region // Definitions - FlexCAN Messages

  FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16> TFTCAN3;


  unsigned int PT_CAN_disp_mode;
  unsigned int PT_CAN_seq_Num=0;
  unsigned int PT_CAN_CF_Num;
  unsigned int PT_CAN_CF_Max;

  unsigned int PT_CAN_Service09_disp_mode;
  unsigned int PT_CAN_Service09_seq_Num;
  unsigned int PT_CAN_Service09_CF_Max;
  unsigned int PT_CAN_Service09_CF_Num;

    // PT_CAN ECU General
  unsigned int PT_CAN_ECU_0x700_STAT_FC_22;  // Flow Control request status
  unsigned int PT_CAN_ECU_0x700_CF_Num;
  unsigned int PT_CAN_ECU_0x700_CF_Max;
  unsigned int PT_CAN_ECU_0x700_seq_Num;
  unsigned int PT_CAN_ECU_0x700_f_sid22_multi;
  
  
  // PT_CAN ECU DTCs
  unsigned int PT_CAN_ECU_0x7E8_STAT_FC_22;  // Flow Control request status
  unsigned int PT_CAN_ECU_0x7E8_CF_Num;
  unsigned int PT_CAN_ECU_0x7E8_CF_Max;
  unsigned int PT_CAN_ECU_0x7E8_seq_Num;
  unsigned int PT_CAN_ECU_0x7E8_f_sid22_multi;
  
  
  // VIN and ECU Identifications
  int PT_CAN_VIN; // service09 0A
  int PT_CAN_ECU_HW_Name; // service09 0A
  int PT_CAN_ECU_CALID; // service09 0A
  
  int PT_CAN_TCU_HW_Name; // service09 0A
  int PT_CAN_TCU_ASW; // service09 0A
  
  unsigned int PT_CAN_VIN_ReadComplete;
  unsigned int PT_CAN_ECU_HW_ReadComplete;
  unsigned int PT_CAN_ECU_CALID_ReadComplete;
  unsigned int PT_CAN_ECU_Variable4_ReadComplete;
  unsigned int PT_CAN_ECU_Variable5_ReadComplete;
  unsigned int PT_CAN_ECU_Variable6_ReadComplete;
  unsigned int PT_CAN_ECU_Variable7_ReadComplete;
  

  unsigned int PT_CAN_TCU_Variable1_ReadComplete;
  unsigned int PT_CAN_TCU_Variable2_ReadComplete;
  unsigned int PT_CAN_TCU_Variable3_ReadComplete;
  unsigned int PT_CAN_TCU_Variable4_ReadComplete;
  unsigned int PT_CAN_TCU_Variable5_ReadComplete;


  unsigned int PT_CAN_f_sid09inftyp;
  unsigned int PT_CAN_f_sid09VIN_multi;
  unsigned int PT_CAN_f_sid09CALID_multi;
  unsigned int PT_CAN_f_sid09ECUName_multi;
  unsigned int PT_CAN_f_sid22_0x700_multi;
  unsigned int PT_CAN_f_sid22_0x7E8_multi;
  unsigned int PT_CAN_f_sid22_0x7E9_multi;
  unsigned int PT_CAN_f_sid22_0x77D_multi;
  // unsigned int PT_CAN_f_sid22_0x77A_multi;


  unsigned int PT_CAN_STAT_FC_01 = 0;  // Flow Control request status
  unsigned int PT_CAN_STAT_FC_03 = 0;  // Flow Control request status
  unsigned int PT_CAN_STAT_FC_04 = 0;  // Flow Control request status
  unsigned int PT_CAN_STAT_FC_09 = 0;  // Flow Control request status
  unsigned int PT_CAN_STAT_FC_22_0x7DC = 0;  // Flow Control request status
  unsigned int PT_CAN_STAT_FC_22_0x7E8 = 0;  // Flow Control request status
  unsigned int PT_CAN_STAT_FC_22_0x7E9 = 0;  // Flow Control request status
  unsigned int PT_CAN_STAT_FC_22_0x779 = 0;  // Flow Control request status
  unsigned int PT_CAN_STAT_FC_22_0x77D = 0;  // Flow Control request status
  unsigned int PT_CAN_STAT_FC_22_0x700 = 0;

  // unsigned int PT_CAN_f_sid22_0x7E8_multi;
  // unsigned int PT_CAN_f_sid22_0x7E9_multi;


  // // PT_CAN ECU DTCs
  // unsigned int PT_CAN_ECU_0x7E8_STAT_FC_22;  // Flow Control request status
  // unsigned int PT_CAN_ECU_0x7E8_CF_Num;
  // unsigned int PT_CAN_ECU_0x7E8_CF_Max;
  // unsigned int PT_CAN_ECU_0x7E8_seq_Num;
  // unsigned int PT_CAN_ECU_0x7E8_f_sid22_multi;



  CAN_message_t   PT_CAN_msg0x780;
  CAN_message_t   PT_CAN_msg0x781_mux1;
  CAN_message_t   PT_CAN_msg0x781_mux2;
  CAN_message_t   PT_CAN_msg0x781_mux3;
  CAN_message_t   PT_CAN_msg0x781_mux4;
  CAN_message_t   PT_CAN_msg0x781_mux5;
  CAN_message_t   PT_CAN_msg0x782;
  CAN_message_t   PT_CAN_msg0x783;
  CAN_message_t   PT_CAN_msg0x784;
  CAN_message_t   PT_CAN_msg0x785;
  CAN_message_t   PT_CAN_msg0x786;
  CAN_message_t   PT_CAN_msg0x787;
  CAN_message_t   PT_CAN_msg0x787_mux1;
  CAN_message_t   PT_CAN_msg0x787_mux2;
  CAN_message_t   PT_CAN_msg0x788_mux1;
  CAN_message_t   PT_CAN_msg0x788_mux2;
  CAN_message_t   PT_CAN_msg0x789;
  CAN_message_t   PT_CAN_msg0x790;
  CAN_message_t   PT_CAN_msg0x791;
  CAN_message_t   PT_CAN_msg0x794_mux1;
  CAN_message_t   PT_CAN_msg0x794_mux2;
  CAN_message_t   PT_CAN_msg0x794_mux3;
  CAN_message_t   PT_CAN_msg0x794_mux4;
  CAN_message_t   PT_CAN_msg0x794_mux5;

  CAN_message_t   PT_CAN_msg0x786_forAIM;
  CAN_message_t   PT_CAN_msg0x787_forAIM;
  CAN_message_t   PT_CAN_msg0x789_forAIM;
  CAN_message_t   PT_CAN_msg0x792_forAIM;
  CAN_message_t   PT_CAN_msg0x793_forAIM;

  CAN_message_t PT_CAN_msg0x1AB; // Brake Pedal Switch Status
  CAN_message_t PT_CAN_msg0xAD; // Gear Lever/Gear Position Status
  CAN_message_t PT_CAN_msg0x120; // Cruise Stalk Static Position

  CAN_message_t PT_CAN_msg0x147; // Power/Torque Display Gauges
  CAN_message_t PT_CAN_msg0x106; // Brake Pressures
  CAN_message_t PT_CAN_msg0x107; // Tachometer on Instrument Cluster - for Tach Override
  CAN_message_t PT_CAN_msg0x670; // Tachometer Shift light in byte7?


  CAN_message_t PT_CAN_msg0x12B; // Cruise Stalk Movement Position/StatusCAN_message_t PT_CAN_msg0x366; // Hazard Lights
  CAN_message_t PT_CAN_msg0x385;
  CAN_message_t PT_CAN_msg0x31B;
  CAN_message_t PT_CAN_msg0x391;
  CAN_message_t PT_CAN_msg0x394;
  CAN_message_t PT_CAN_msg0x3DD; // Paddle position/Shift status

  CAN_message_t PT_CAN_msg0x0A8; // Engine RPM

  CAN_message_t PT_CAN_msg0xB2; // Wheel Speed Sensors

  CAN_message_t PT_CAN_msg0x700;
  CAN_message_t PT_CAN_msg0x7DC;
  CAN_message_t PT_CAN_msg0x7EE;
  CAN_message_t PT_CAN_msg0x779;
  CAN_message_t PT_CAN_msg09;

  word cruiseStalkCAN;
  word cruiseStalk_byte1CAN;

  CAN_message_t PT_CAN_msg0x7E8;


  unsigned int PT_CAN_msg0x147_buf0, PT_CAN_msg0x147_buf1, PT_CAN_msg0x147_buf2, PT_CAN_msg0x147_buf3, PT_CAN_msg0x147_buf4, PT_CAN_msg0x147_buf5, PT_CAN_msg0x147_buf6, PT_CAN_msg0x147_buf7;
  unsigned int PT_CAN_msg0x3DD_buf0, PT_CAN_msg0x3DD_buf1, PT_CAN_msg0x3DD_buf2, PT_CAN_msg0x3DD_buf3, PT_CAN_msg0x3DD_buf4, PT_CAN_msg0x3DD_buf5, PT_CAN_msg0x3DD_buf6, PT_CAN_msg0x3DD_buf7; // 0x3DD is for paddle positions/shift status
  unsigned int PT_CAN_msg0xB2x1buf0, PT_CAN_msg0xB2x1buf1, PT_CAN_msg0xB2x1buf2, PT_CAN_msg0xB2x1buf3, PT_CAN_msg0xB2x1buf4, PT_CAN_msg0xB2x1buf5, PT_CAN_msg0xB2x1buf6, PT_CAN_msg0xB2x1buf7;
  unsigned int PT_CAN_msg0x86_buf0, PT_CAN_msg0x86_buf1, PT_CAN_msg0x86_buf2, PT_CAN_msg0x86_buf3, PT_CAN_msg0x86_buf4, PT_CAN_msg0x86_buf5, PT_CAN_msg0x86_buf6, PT_CAN_msg0x86_buf7;

  unsigned int PT_CAN_msg0x700_buf0, PT_CAN_msg0x700_buf1, PT_CAN_msg0x700_buf2, PT_CAN_msg0x700_buf3, PT_CAN_msg0x700_buf4, PT_CAN_msg0x700_buf5, PT_CAN_msg0x700_buf6, PT_CAN_msg0x700_buf7;
  unsigned int PT_CAN_msg0x700_CF20_buf0, PT_CAN_msg0x700_CF20_buf1, PT_CAN_msg0x700_CF20_buf2, PT_CAN_msg0x700_CF20_buf3, PT_CAN_msg0x700_CF20_buf4, PT_CAN_msg0x700_CF20_buf5, PT_CAN_msg0x700_CF20_buf6, PT_CAN_msg0x700_CF20_buf7;
  unsigned int PT_CAN_msg0x700_CF21_buf0, PT_CAN_msg0x700_CF21_buf1, PT_CAN_msg0x700_CF21_buf2, PT_CAN_msg0x700_CF21_buf3, PT_CAN_msg0x700_CF21_buf4, PT_CAN_msg0x700_CF21_buf5, PT_CAN_msg0x700_CF21_buf6, PT_CAN_msg0x700_CF21_buf7;


  unsigned int PT_CAN_msg0x7E8_buf0, PT_CAN_msg0x7E8_buf1, PT_CAN_msg0x7E8_buf2, PT_CAN_msg0x7E8_buf3, PT_CAN_msg0x7E8_buf4, PT_CAN_msg0x7E8_buf5, PT_CAN_msg0x7E8_buf6, PT_CAN_msg0x7E8_buf7;
  unsigned int PT_CAN_msg0x7E8_CF20_buf0, PT_CAN_msg0x7E8_CF20_buf1, PT_CAN_msg0x7E8_CF20_buf2, PT_CAN_msg0x7E8_CF20_buf3, PT_CAN_msg0x7E8_CF20_buf4, PT_CAN_msg0x7E8_CF20_buf5, PT_CAN_msg0x7E8_CF20_buf6, PT_CAN_msg0x7E8_CF20_buf7;
  unsigned int PT_CAN_msg0x7E8_CF21_buf0, PT_CAN_msg0x7E8_CF21_buf1, PT_CAN_msg0x7E8_CF21_buf2, PT_CAN_msg0x7E8_CF21_buf3, PT_CAN_msg0x7E8_CF21_buf4, PT_CAN_msg0x7E8_CF21_buf5, PT_CAN_msg0x7E8_CF21_buf6, PT_CAN_msg0x7E8_CF21_buf7;

  unsigned int PT_CAN_msg0x7E9_buf0, PT_CAN_msg0x7E9_buf1, PT_CAN_msg0x7E9_buf2, PT_CAN_msg0x7E9_buf3, PT_CAN_msg0x7E9_buf4, PT_CAN_msg0x7E9_buf5, PT_CAN_msg0x7E9_buf6, PT_CAN_msg0x7E9_buf7;
  unsigned int PT_CAN_msg0x7E9_CF20_buf0, PT_CAN_msg0x7E9_CF20_buf1, PT_CAN_msg0x7E9_CF20_buf2, PT_CAN_msg0x7E9_CF20_buf3, PT_CAN_msg0x7E9_CF20_buf4, PT_CAN_msg0x7E9_CF20_buf5, PT_CAN_msg0x7E9_CF20_buf6, PT_CAN_msg0x7E9_CF20_buf7;
  unsigned int PT_CAN_msg0x7E9_CF21_buf0, PT_CAN_msg0x7E9_CF21_buf1, PT_CAN_msg0x7E9_CF21_buf2, PT_CAN_msg0x7E9_CF21_buf3, PT_CAN_msg0x7E9_CF21_buf4, PT_CAN_msg0x7E9_CF21_buf5, PT_CAN_msg0x7E9_CF21_buf6, PT_CAN_msg0x7E9_CF21_buf7;
  unsigned int PT_CAN_msg0x7E9_CF22_buf0, PT_CAN_msg0x7E9_CF22_buf1, PT_CAN_msg0x7E9_CF22_buf2, PT_CAN_msg0x7E9_CF22_buf3, PT_CAN_msg0x7E9_CF22_buf4, PT_CAN_msg0x7E9_CF22_buf5, PT_CAN_msg0x7E9_CF22_buf6, PT_CAN_msg0x7E9_CF22_buf7;
  unsigned int PT_CAN_msg0x7E9_CF23_buf0, PT_CAN_msg0x7E9_CF23_buf1, PT_CAN_msg0x7E9_CF23_buf2, PT_CAN_msg0x7E9_CF23_buf3, PT_CAN_msg0x7E9_CF23_buf4, PT_CAN_msg0x7E9_CF23_buf5, PT_CAN_msg0x7E9_CF23_buf6, PT_CAN_msg0x7E9_CF23_buf7;




  // ECU 0x7E8 CAN Frame buffer variables from PT_CAN Sniffing
  word PT_CAN_msg0x7E8_buf0_Raw, PT_CAN_msg0x7E8_buf1_Raw, PT_CAN_msg0x7E8_buf2_Raw, PT_CAN_msg0x7E8_buf3_Raw, PT_CAN_msg0x7E8_buf4_Raw, PT_CAN_msg0x7E8_buf5_Raw, PT_CAN_msg0x7E8_buf6_Raw, PT_CAN_msg0x7E8_buf7_Raw;

  // TCU 0x7E9 CAN Frame buffer variables from PT_CAN Sniffing
  word PT_CAN_msg0x7E9_buf0_Raw, PT_CAN_msg0x7E9_buf1_Raw, PT_CAN_msg0x7E9_buf2_Raw, PT_CAN_msg0x7E9_buf3_Raw, PT_CAN_msg0x7E9_buf4_Raw, PT_CAN_msg0x7E9_buf5_Raw, PT_CAN_msg0x7E9_buf6_Raw, PT_CAN_msg0x7E9_buf7_Raw;
  word PT_CAN_msg0x7E9_x1buf0_Raw, PT_CAN_msg0x7E9_x1buf1_Raw, PT_CAN_msg0x7E9_x1buf2_Raw, PT_CAN_msg0x7E9_x1buf3_Raw, PT_CAN_msg0x7E9_x1buf4_Raw, PT_CAN_msg0x7E9_x1buf5_Raw, PT_CAN_msg0x7E9_x1buf6_Raw, PT_CAN_msg0x7E9_x1buf7_Raw;
  word PT_CAN_msg0x7E9_x21buf0_Raw, PT_CAN_msg0x7E9_x21buf1_Raw, PT_CAN_msg0x7E9_x21buf2_Raw, PT_CAN_msg0x7E9_x21buf3_Raw, PT_CAN_msg0x7E9_x21buf4_Raw, PT_CAN_msg0x7E9_x21buf5_Raw, PT_CAN_msg0x7E9_x21buf6_Raw, PT_CAN_msg0x7E9_x21buf7_Raw;
  word PT_CAN_msg0x7E9_x22buf0_Raw, PT_CAN_msg0x7E9_x22buf1_Raw, PT_CAN_msg0x7E9_x22buf2_Raw, PT_CAN_msg0x7E9_x22buf3_Raw, PT_CAN_msg0x7E9_x22buf4_Raw, PT_CAN_msg0x7E9_x22buf5_Raw, PT_CAN_msg0x7E9_x22buf6_Raw, PT_CAN_msg0x7E9_x22buf7_Raw;
  word PT_CAN_msg0x7E9_x23buf0_Raw, PT_CAN_msg0x7E9_x23buf1_Raw, PT_CAN_msg0x7E9_x23buf2_Raw, PT_CAN_msg0x7E9_x23buf3_Raw, PT_CAN_msg0x7E9_x23buf4_Raw, PT_CAN_msg0x7E9_x23buf5_Raw, PT_CAN_msg0x7E9_x23buf6_Raw, PT_CAN_msg0x7E9_x23buf7_Raw;
  word PT_CAN_msg0x7E9_x24buf0_Raw, PT_CAN_msg0x7E9_x24buf1_Raw, PT_CAN_msg0x7E9_x24buf2_Raw, PT_CAN_msg0x7E9_x24buf3_Raw, PT_CAN_msg0x7E9_x24buf4_Raw, PT_CAN_msg0x7E9_x24buf5_Raw, PT_CAN_msg0x7E9_x24buf6_Raw, PT_CAN_msg0x7E9_x24buf7_Raw;


  word PT_CAN_msg0x40_buf0_Raw, PT_CAN_msg0x40_buf1_Raw, PT_CAN_msg0x40_buf2_Raw, PT_CAN_msg0x40_buf3_Raw, PT_CAN_msg0x40_buf4_Raw, PT_CAN_msg0x40_buf5_Raw, PT_CAN_msg0x40_buf6_Raw, PT_CAN_msg0x40_buf7_Raw;
  word PT_CAN_msg0xA8_buf0_Raw, PT_CAN_msg0xA8_buf1_Raw, PT_CAN_msg0xA8_buf2_Raw, PT_CAN_msg0xA8_buf3_Raw, PT_CAN_msg0xA8_buf4_Raw, PT_CAN_msg0xA8_buf5_Raw, PT_CAN_msg0xA8_buf6_Raw, PT_CAN_msg0xA8_buf7_Raw;
  word PT_CAN_msg0xAD_buf0_Raw, PT_CAN_msg0xAD_buf1_Raw, PT_CAN_msg0xAD_buf2_Raw, PT_CAN_msg0xAD_buf3_Raw, PT_CAN_msg0xAD_buf4_Raw, PT_CAN_msg0xAD_buf5_Raw, PT_CAN_msg0xAD_buf6_Raw, PT_CAN_msg0xAD_buf7_Raw;
  word PT_CAN_msg0xB2_buf0_Raw, PT_CAN_msg0xB2_buf1_Raw, PT_CAN_msg0xB2_buf2_Raw, PT_CAN_msg0xB2_buf3_Raw, PT_CAN_msg0xB2_buf4_Raw, PT_CAN_msg0xB2_buf5_Raw, PT_CAN_msg0xB2_buf6_Raw, PT_CAN_msg0xB2_buf7_Raw;
  word PT_CAN_msg0x86_buf0_Raw, PT_CAN_msg0x86_buf1_Raw, PT_CAN_msg0x86_buf2_Raw, PT_CAN_msg0x86_buf3_Raw, PT_CAN_msg0x86_buf4_Raw, PT_CAN_msg0x86_buf5_Raw, PT_CAN_msg0x86_buf6_Raw, PT_CAN_msg0x86_buf7_Raw;
  word PT_CAN_msg0x104_buf0_Raw, PT_CAN_msg0x104_buf1_Raw, PT_CAN_msg0x104_buf2_Raw, PT_CAN_msg0x104_buf3_Raw, PT_CAN_msg0x104_buf4_Raw, PT_CAN_msg0x104_buf5_Raw, PT_CAN_msg0x104_buf6_Raw, PT_CAN_msg0x104_buf7_Raw;
  word PT_CAN_msg0x106_buf0_Raw, PT_CAN_msg0x106_buf1_Raw, PT_CAN_msg0x106_buf2_Raw, PT_CAN_msg0x106_buf3_Raw, PT_CAN_msg0x106_buf4_Raw, PT_CAN_msg0x106_buf5_Raw, PT_CAN_msg0x106_buf6_Raw, PT_CAN_msg0x106_buf7_Raw;
  word PT_CAN_msg0x106_AIMtx_buf0_Raw, PT_CAN_msg0x106_AIMtx_buf1_Raw, PT_CAN_msg0x106_AIMtx_buf2_Raw, PT_CAN_msg0x106_AIMtx_buf3_Raw, PT_CAN_msg0x106_AIMtx_buf4_Raw, PT_CAN_msg0x106_AIMtx_buf5_Raw, PT_CAN_msg0x106_AIMtx_buf6_Raw, PT_CAN_msg0x106_AIMtx_buf7_Raw;
  word PT_CAN_msg0x107_buf0_Raw, PT_CAN_msg0x107_buf1_Raw, PT_CAN_msg0x107_buf2_Raw, PT_CAN_msg0x107_buf3_Raw, PT_CAN_msg0x107_buf4_Raw, PT_CAN_msg0x107_buf5_Raw, PT_CAN_msg0x107_buf6_Raw, PT_CAN_msg0x107_buf7_Raw;
  word PT_CAN_msg0x120_buf0_Raw, PT_CAN_msg0x120_buf1_Raw, PT_CAN_msg0x120_buf2_Raw, PT_CAN_msg0x120_buf3_Raw, PT_CAN_msg0x120_buf4_Raw, PT_CAN_msg0x120_buf5_Raw, PT_CAN_msg0x120_buf6_Raw, PT_CAN_msg0x120_buf7_Raw;
  word PT_CAN_msg0x12B_buf0_Raw, PT_CAN_msg0x12B_buf1_Raw, PT_CAN_msg0x12B_buf2_Raw, PT_CAN_msg0x12B_buf3_Raw, PT_CAN_msg0x12B_buf4_Raw, PT_CAN_msg0x12B_buf5_Raw, PT_CAN_msg0x12B_buf6_Raw, PT_CAN_msg0x12B_buf7_Raw;
  word PT_CAN_msg0x147_buf0_Raw, PT_CAN_msg0x147_buf1_Raw, PT_CAN_msg0x147_buf2_Raw, PT_CAN_msg0x147_buf3_Raw, PT_CAN_msg0x147_buf4_Raw, PT_CAN_msg0x147_buf5_Raw, PT_CAN_msg0x147_buf6_Raw, PT_CAN_msg0x147_buf7_Raw;
  word PT_CAN_msg0x1AB_buf0_Raw, PT_CAN_msg0x1AB_buf1_Raw, PT_CAN_msg0x1AB_buf2_Raw, PT_CAN_msg0x1AB_buf3_Raw, PT_CAN_msg0x1AB_buf4_Raw, PT_CAN_msg0x1AB_buf5_Raw, PT_CAN_msg0x1AB_buf6_Raw, PT_CAN_msg0x1AB_buf7_Raw;
  word PT_CAN_msg0x366_buf0_Raw, PT_CAN_msg0x366_buf1_Raw, PT_CAN_msg0x366_buf2_Raw, PT_CAN_msg0x366_buf3_Raw, PT_CAN_msg0x366_buf4_Raw, PT_CAN_msg0x366_buf5_Raw, PT_CAN_msg0x366_buf6_Raw, PT_CAN_msg0x366_buf7_Raw;
  word PT_CAN_msg0x385_buf0_Raw, PT_CAN_msg0x385_buf1_Raw, PT_CAN_msg0x385_buf2_Raw, PT_CAN_msg0x385_buf3_Raw, PT_CAN_msg0x385_buf4_Raw, PT_CAN_msg0x385_buf5_Raw, PT_CAN_msg0x385_buf6_Raw, PT_CAN_msg0x385_buf7_Raw;
  word PT_CAN_msg0x31B_buf0_Raw, PT_CAN_msg0x31B_buf1_Raw, PT_CAN_msg0x31B_buf2_Raw, PT_CAN_msg0x31B_buf3_Raw, PT_CAN_msg0x31B_buf4_Raw, PT_CAN_msg0x31B_buf5_Raw, PT_CAN_msg0x31B_buf6_Raw, PT_CAN_msg0x31B_buf7_Raw;
  word PT_CAN_msg0x391_buf0_Raw, PT_CAN_msg0x391_buf1_Raw, PT_CAN_msg0x391_buf2_Raw, PT_CAN_msg0x391_buf3_Raw, PT_CAN_msg0x391_buf4_Raw, PT_CAN_msg0x391_buf5_Raw, PT_CAN_msg0x391_buf6_Raw, PT_CAN_msg0x391_buf7_Raw;
  word PT_CAN_msg0x394_buf0_Raw, PT_CAN_msg0x394_buf1_Raw, PT_CAN_msg0x394_buf2_Raw, PT_CAN_msg0x394_buf3_Raw, PT_CAN_msg0x394_buf4_Raw, PT_CAN_msg0x394_buf5_Raw, PT_CAN_msg0x394_buf6_Raw, PT_CAN_msg0x394_buf7_Raw;
  word PT_CAN_msg0x3DD_buf0_Raw, PT_CAN_msg0x3DD_buf1_Raw, PT_CAN_msg0x3DD_buf2_Raw, PT_CAN_msg0x3DD_buf3_Raw, PT_CAN_msg0x3DD_buf4_Raw, PT_CAN_msg0x3DD_buf5_Raw, PT_CAN_msg0x3DD_buf6_Raw, PT_CAN_msg0x3DD_buf7_Raw; // 0x3DD is for paddle positions/shift status
  word PT_CAN_msg0x743_buf0_Raw, PT_CAN_msg0x743_buf1_Raw, PT_CAN_msg0x743_buf2_Raw, PT_CAN_msg0x743_buf3_Raw, PT_CAN_msg0x743_buf4_Raw, PT_CAN_msg0x743_buf5_Raw, PT_CAN_msg0x743_buf6_Raw, PT_CAN_msg0x743_buf7_Raw;
  word PT_CAN_msg0x783_buf0_Raw, PT_CAN_msg0x783_buf1_Raw, PT_CAN_msg0x783_buf2_Raw, PT_CAN_msg0x783_buf3_Raw, PT_CAN_msg0x783_buf4_Raw, PT_CAN_msg0x783_buf5_Raw, PT_CAN_msg0x783_buf6_Raw, PT_CAN_msg0x783_buf7_Raw;
  word PT_CAN_msg0x788_buf0_Raw, PT_CAN_msg0x788_buf1_Raw, PT_CAN_msg0x788_buf2_Raw, PT_CAN_msg0x788_buf3_Raw, PT_CAN_msg0x788_buf4_Raw, PT_CAN_msg0x788_buf5_Raw, PT_CAN_msg0x788_buf6_Raw, PT_CAN_msg0x788_buf7_Raw;
  word PT_CAN_msg0x789_buf0_Raw, PT_CAN_msg0x789_buf1_Raw, PT_CAN_msg0x789_buf2_Raw, PT_CAN_msg0x789_buf3_Raw, PT_CAN_msg0x789_buf4_Raw, PT_CAN_msg0x789_buf5_Raw, PT_CAN_msg0x789_buf6_Raw, PT_CAN_msg0x789_buf7_Raw;
  word PT_CAN_msg0x790_buf0_Raw, PT_CAN_msg0x790_buf1_Raw, PT_CAN_msg0x790_buf2_Raw, PT_CAN_msg0x790_buf3_Raw, PT_CAN_msg0x790_buf4_Raw, PT_CAN_msg0x790_buf5_Raw, PT_CAN_msg0x790_buf6_Raw, PT_CAN_msg0x790_buf7_Raw;
  word PT_CAN_msg0x791_buf0_Raw, PT_CAN_msg0x791_buf1_Raw, PT_CAN_msg0x791_buf2_Raw, PT_CAN_msg0x791_buf3_Raw, PT_CAN_msg0x791_buf4_Raw, PT_CAN_msg0x791_buf5_Raw, PT_CAN_msg0x791_buf6_Raw, PT_CAN_msg0x791_buf7_Raw;
  word PT_CAN_msg0x792_buf0_Raw, PT_CAN_msg0x792_buf1_Raw, PT_CAN_msg0x792_buf2_Raw, PT_CAN_msg0x792_buf3_Raw, PT_CAN_msg0x792_buf4_Raw, PT_CAN_msg0x792_buf5_Raw, PT_CAN_msg0x792_buf6_Raw, PT_CAN_msg0x792_buf7_Raw;
  word PT_CAN_msg0x793_buf0_Raw, PT_CAN_msg0x793_buf1_Raw, PT_CAN_msg0x793_buf2_Raw, PT_CAN_msg0x793_buf3_Raw, PT_CAN_msg0x793_buf4_Raw, PT_CAN_msg0x793_buf5_Raw, PT_CAN_msg0x793_buf6_Raw, PT_CAN_msg0x793_buf7_Raw;
  word PT_CAN_msg0x794_buf0_Raw, PT_CAN_msg0x794_buf1_Raw, PT_CAN_msg0x794_buf2_Raw, PT_CAN_msg0x794_buf3_Raw, PT_CAN_msg0x794_buf4_Raw, PT_CAN_msg0x794_buf5_Raw, PT_CAN_msg0x794_buf6_Raw, PT_CAN_msg0x794_buf7_Raw;
  word PT_CAN_msg0x795_buf0_Raw, PT_CAN_msg0x795_buf1_Raw, PT_CAN_msg0x795_buf2_Raw, PT_CAN_msg0x795_buf3_Raw, PT_CAN_msg0x795_buf4_Raw, PT_CAN_msg0x795_buf5_Raw, PT_CAN_msg0x795_buf6_Raw, PT_CAN_msg0x795_buf7_Raw;
  word PT_CAN_msg0x796_buf0_Raw, PT_CAN_msg0x796_buf1_Raw, PT_CAN_msg0x796_buf2_Raw, PT_CAN_msg0x796_buf3_Raw, PT_CAN_msg0x796_buf4_Raw, PT_CAN_msg0x796_buf5_Raw, PT_CAN_msg0x796_buf6_Raw, PT_CAN_msg0x796_buf7_Raw;

  word PT_CAN_msg0x7EE_buf0_Raw, PT_CAN_msg0x7EE_buf1_Raw, PT_CAN_msg0x7EE_buf2_Raw, PT_CAN_msg0x7EE_buf3_Raw, PT_CAN_msg0x7EE_buf4_Raw, PT_CAN_msg0x7EE_buf5_Raw, PT_CAN_msg0x7EE_buf6_Raw, PT_CAN_msg0x7EE_buf7_Raw;


  #pragma endregion

#pragma region // Define CAN Variables sniffed from aftermarket ECUs/Controllers (Syvecs AWD, TFT, Fuel Pumps, widebands, etc)


  word SyvecsAWD_AWDBase_CAN;
  word SyvecsAWD_AWDFinal_CAN;
  word SyvecsAWD_AWDPot_CAN;
  word SyvecsAWD_AWDPotMultiplier_CAN;
  word SyvecsAWD_OutputCurrent;

  word SyvecsAWD_FrontSlipCalc_CAN;
  word SyvecsAWD_RearSlipCalc_CAN;
  word SyvecsAWD_CenterSlipCalc_CAN;
  word SyvecsAWD_FrontSlipAdder_CAN;
  word SyvecsAWD_RearSlipAdder_CAN;
  word SyvecsAWD_CenterSlipAdder_CAN;

  word SyvecsAWD_SteeringWheelAngle_CAN;
  word SyvecsAWD_SteeringWheelAngleAdder_CAN;
  word SyvecsAWD_SteeringWheelAngleMultiplier_CAN;

  word SyvecsAWD_BrakePressure_CAN;
  word SyvecsAWD_BrakePressureAdder_CAN;
  word SyvecsAWD_BrakePressureAdderMultiplier_CAN;


  // ----------------------------------------------------------

  unsigned char SyvecsAWD_AWDBase;
  unsigned char SyvecsAWD_AWDFinal;
  unsigned char SyvecsAWD_AWDPot;
  unsigned char SyvecsAWD_AWDPotMultiplier;

  unsigned char SyvecsAWD_FrontSlipCalc;
  unsigned char SyvecsAWD_RearSlipCalc;
  unsigned char SyvecsAWD_CenterSlipCalc;
  unsigned char SyvecsAWD_FrontSlipAdder;
  unsigned char SyvecsAWD_RearSlipAdder;
  unsigned char SyvecsAWD_CenterSlipAdder;


  unsigned char SyvecsAWD_SteeringWheelAngle;
  unsigned char SyvecsAWD_SteeringWheelAngleMultiplier;
  unsigned char SyvecsAWD_SteeringWheelAngleAdder;

  unsigned char SyvecsAWD_BrakePressure;
  unsigned char SyvecsAWD_BrakePressureMultiplier;
  unsigned char SyvecsAWD_BrakePressureAdder;




  // ----------------------------------------------------------


  unsigned int SyvecsAWD_AWDBase_Int;
  unsigned int SyvecsAWD_AWDFinal_Int;
  unsigned int SyvecsAWD_AWDPot_Int;
  unsigned int SyvecsAWD_AWDPotMultiplier_Int;


  unsigned int SyvecsAWD_FrontSlipCalc_Int;
  unsigned int SyvecsAWD_RearSlipCalc_Int;
  unsigned int SyvecsAWD_CenterSlipCalc_Int;
  unsigned int SyvecsAWD_FrontSlipAdder_Int;
  unsigned int SyvecsAWD_RearSlipAdder_Int;
  unsigned int SyvecsAWD_CenterSlipAdder_Int;



  unsigned int SyvecsAWD_SteeringWheelAngle_Int;
  unsigned int SyvecsAWD_SteeringWheelAngleMultiplier_Int;
  unsigned int SyvecsAWD_SteeringWheelAngleAdder_Int;

  unsigned int SyvecsAWD_BrakePressureBAR_Int;
  unsigned int SyvecsAWD_BrakePressureMultiplier_Int;
  unsigned int SyvecsAWD_BrakePressureAdder_Int;



  // ----------------------------------------------------------


  float SyvecsAWD_AWDBaseFloat;
  float SyvecsAWD_AWDFinalFloat;
  float SyvecsAWD_AWDPotFloat;
  float SyvecsAWD_AWDPotMultiplier_Float;

  float SyvecsAWD_FrontSlipCalc_Float;
  float SyvecsAWD_RearSlipCalc_Float;
  float SyvecsAWD_CenterSlipCalc_Float;
  float SyvecsAWD_FrontSlipAdder_Float;
  float SyvecsAWD_RearSlipAdder_Float;
  float SyvecsAWD_CenterSlipAdder_Float;

  float SyvecsAWD_SteeringWheelAngle_Float;
  float SyvecsAWD_SteeringWheelAngleMultiplier_Float;
  float SyvecsAWD_SteeringWheelAngleAdder_Float;

  float SyvecsAWD_BrakePressure_Float;
  float SyvecsAWD_BrakePressureMultiplier_Float;
  float SyvecsAWD_BrakePressureAdder_Float;





  // ----------------------------------------------------------





#pragma endregion

#pragma region // Define Native PT_CAN MQB Variables used for Executable CAN Functions - PT_CAN - Operational and Driver Inputs
  

  unsigned int DS1CurrentMapSlot_0x7EE; 
  unsigned int PowerGauge_Converted;
  unsigned int TorqueGauge_Converted;

  unsigned int HazardLightsButton;

  unsigned int BrakePedalSwitchStatus;

  word BrakePressureBAR_0x106_CAN;
  word BrakePressureBAR_0x106_Extracted;
  unsigned int BrakePressureBAR_0x106_Raw;
  unsigned int BrakePressureBAR_Int;
  float BrakePressureBAR_Float;

  word MQB_SteeringWheelAngle_0x86_CAN;
  word MQB_SteeringWheelAngle_0x86_Extracted;
  unsigned int MQB_SteeringWheelAngle_0x86_Raw;
  float MQB_SteeringWheelAngle_0x86_Float;
  unsigned int MQB_SteeringWheelAngle_0x86_Int;
  float MQB_SteeringWheelAngle_0x86_Radians_Int;


  word ThrottleTPS_0x391_CAN;
  word AccelPedal_0x391_CAN;
  unsigned int AccelPedal;
  unsigned int AccelPedal_0x107;
  unsigned int AccelPedal_0x391;
  unsigned int AccelPedal_0x391_Mapped;
  float AccelPedal_0x391_Mapped_Float;

  unsigned int ThrottleTPS_0x391;


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


  unsigned int cruiseStalkStaticPosition_0x120_bit;
  // unsigned int CruiseStalk_OFF_bit;
  unsigned int CruiseStalk_ON_bit;
  unsigned int CruiseStalk_UP_bit;
  unsigned int CruiseStalk_DOWN_bit;
  unsigned int CruiseStalk_RES_bit;
  unsigned int CruiseStalk_FORWARD_bit;
  unsigned int CruiseStalk_SET_bit;
  unsigned int CruiseStalk_DistanceUP_bit;
  unsigned int CruiseStalk_DistanceDOWN_bit;


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
  unsigned int cruiseStalk_byte1;
  unsigned int cruiseStalk_byte2;
  unsigned int WGDCPassthrough;
  unsigned int TQIntvStatus;

  unsigned int PaddlePositions;
  unsigned int BrakeSwitch_0x1AB;
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

  int VehSpeed;
  int WhlSpdFR;
  int WhlSpdFL;
  int WhlSpdRL;
  int WhlSpdRR;
  int WhlSpdAvg_Int;

  int WhlSpdFR_Raw;
  int WhlSpdFL_Raw;
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



#pragma endregion

#pragma region // Declarations of PT_CAN Timers

  elapsedMillis PT_CAN_canTxInterval_0x700;
  elapsedMillis PT_CAN_canTxInterval_0x780;
  elapsedMillis PT_CAN_canTxInterval_0x781;
  elapsedMillis PT_CAN_canTxInterval_0x782;
  elapsedMillis PT_CAN_canTxInterval_0x783;
  elapsedMillis PT_CAN_canTxInterval_0x784;
  elapsedMillis PT_CAN_canTxInterval_0x785;
  elapsedMillis PT_CAN_canTxInterval_0x785_mux2;
  elapsedMillis PT_CAN_canTxInterval_0x786_mux1;
  elapsedMillis PT_CAN_canTxInterval_0x786_mux2;
  elapsedMillis PT_CAN_canTxInterval_0x786_mux3;
  elapsedMillis PT_CAN_canTxInterval_0x787_mux1;
  elapsedMillis PT_CAN_canTxInterval_0x787_mux2;
  elapsedMillis PT_CAN_canTxInterval_0x787_mux3;
  elapsedMillis PT_CAN_canTxInterval_0x788_mux1;
  elapsedMillis PT_CAN_canTxInterval_0x788_mux2;
  elapsedMillis PT_CAN_canTxInterval_0x789;
  
  elapsedMillis PT_CAN_canTxInterval_0x791;
  elapsedMillis PT_CAN_canTxInterval_0x792;
  elapsedMillis PT_CAN_canTxInterval_0x793;
  elapsedMillis PT_CAN_canTxInterval_0x794;

  elapsedMillis PT_CAN_canTxInterval_0x786_forAIM;
  elapsedMillis PT_CAN_canTxInterval_0x787_forAIM;
  elapsedMillis PT_CAN_canTxInterval_0x789_forAIM;
  elapsedMillis PT_CAN_canTxInterval_0x790_forAIM;
  elapsedMillis PT_CAN_canTxInterval_0x791_forAIM;
  elapsedMillis PT_CAN_canTxInterval_0x792_forAIM;
  elapsedMillis PT_CAN_canTxInterval_0x793_forAIM;


  elapsedMillis PT_CAN_msg0x7E9_WakeupResponse_TimeSinceResponse;
  elapsedMillis PT_CAN_msg0x7E8_WakeupResponse_TimeSinceResponse;

  elapsedMicros PT_CAN_canTxInterval_0x147; // DisplayGauges Override, in Micros
  elapsedMicros PT_CAN_canTxInterval_0x107; // Tachometer Override, in Micros

  // elapsedMillis loopTimer_20ms;

  elapsedMillis ShutdownTimer;
  elapsedMillis BOVTimer;
  elapsedMillis BOVResetDelay;
  elapsedMillis IgnitionStatusTimer_TimeSinceLastMessage;
  elapsedMillis IgnitionStatusTimer_TimeSinceIgnitionON;


  elapsedMillis loopDelayBasicDelayfromStart; 

  elapsedMillis loopDelay4; 
  elapsedMillis loopDelay5; 
  elapsedMillis loopDelay6; 
  elapsedMillis loopDelayGearCheck; 

  elapsedMillis CruiseStalk_RES_Timer;
  elapsedMillis CruiseStalk_FORWARD_Timer;
  elapsedMillis CruiseStalk_UP_Timer;
  elapsedMillis CruiseStalk_DOWN_Timer;
  elapsedMillis CruiseStalk_SET_Timer;
  elapsedMillis ConfigurationUpdateMode1_TimeoutTimer;

  elapsedMillis CruiseStalk_RES_DebounceTimer;
  elapsedMillis CruiseStalk_FORWARD_DebounceTimer;
  elapsedMillis CruiseStalk_UP_DebounceTimer;
  elapsedMillis CruiseStalk_DOWN_DebounceTimer;
  elapsedMillis CruiseStalk_SET_DebounceTimer;



  elapsedMillis loopDelayPT_CAN_KeepAwake_0x700;
  elapsedMillis loopDelayPT_CAN_DTC_Clear;
  elapsedMillis loopDelayPT_CAN_Service09;
  elapsedMillis loopDelayPT_CAN_Service22;


  elapsedMillis loopDelayPT_CAN_LPFP_ContinuousSequence;
  elapsedMillis loopDelayPT_CAN_LPFP_Activation;
  elapsedMillis loopDelayPT_CAN_LPFP_DEActivation;

  elapsedMillis PT_CAN_LPFP_Activation_TimeSinceSignal;
  elapsedMillis PT_CAN_LPFP_DEActivation_TimeSinceSignal;
  elapsedMillis PT_CAN_LPFP_Activation_Continuous_FirstMessage_TimeSinceSignal;
  elapsedMillis PT_CAN_LPFP_Activation_Continuous_SecondMessage_TimeSinceSignal;


  elapsedMillis TimeSinceWhlSpd0;
  elapsedMillis loopDelayAutoShift_RPMSetpoint_ChangeRPM;
  elapsedMillis loopDelayAutoUPshift_RPMSetpoint_SetRPM;

  elapsedMillis loopDelayAutoShift_GearShiftToUpdate;
  elapsedMillis loopDelayTFTConfig_VariableUpdate;

  elapsedMillis MQB_LaunchControl_EngineRPM_RAMP_Mode_Timer;

  // elapsedMillis GaugeTimer1;
  // elapsedMillis GaugeTimer2;
  // elapsedMillis GaugeTimer3;
  // elapsedMillis GaugeTimer4;
  // elapsedMillis GaugeTimerStatic;
  // // elapsedMillis IMUCalibrationDelay;

  elapsedMillis PaddleUP_SignalDebounceDelay;
  elapsedMillis PaddleDOWN_SignalDebounceDelay;

  unsigned int PaddleUP_TimeSinceSignal;
  unsigned int PaddleDOWN_TimeSinceSignal;

  // elapsedMillis loopDelayLevelSensors; 
  // elapsedMillis loopDelayHaldexSet1; 
  // elapsedMillis loopDelayHaldexSet2; 


  const long loopDelay1 = 50; // Make a request every 500ms
  const long loopDelay2 = 2000; //
  const long loopDelay3 = 500; // 


  unsigned long timeOffset18 = 50;
  unsigned long timeOffset19 = 100;
  unsigned long timeOffset20 = 400;


  unsigned long timeOffset28 = 5000;
  unsigned long timeOffset29 = 2000;
  unsigned long timeOffset30 = 30000;





  #pragma endregion

#pragma region // Declarations of Variables used by PT-CAN Requests and Injection Functions generated by TFT (DQ500 Resets, AutoUPshifts, AutoDOWNshifts, etc)

    #pragma region // Switches to activate OEM Service Requests (Service01/Service09 via PT_CAN

      unsigned int Switch_Req_PT_CAN_09_VIN;
      unsigned int Switch_Req_PT_CAN_09_CALID;
      unsigned int Switch_Req_PT_CAN_09_ECUName;


    #pragma endregion
    
    #pragma region // Variables for DQ500 Basic Settings Resets Activation Functions

    bool DQ500Reset_ClutchCharacteristicCurve_HasRun = false;
    unsigned int DQ500Reset_ClutchCharacteristicCurve_Allowed = 1;
    unsigned int DQ500Reset_ClutchCharacteristicCurve_Trigger = 0;

    bool DQ500Reset_DblClutchFastAdaptation_HasRun = false;
    unsigned int DQ500Reset_DblClutchFastAdaptation_Allowed = 1;
    unsigned int DQ500Reset_DblClutchFastAdaptation_Trigger = 0;

    bool DQ500Reset_ResetBasicSettings_HasRun = false;
    unsigned int DQ500Reset_ResetBasicSettings_Allowed = 1;
    unsigned int DQ500Reset_ResetBasicSettings_Trigger = 0;
    

    unsigned int DQ500Reset_BrakePedalAcceptableCondition = 0;
    unsigned int DQ500Reset_EngRPMAcceptableCondition = 0;
    unsigned int DQ500Reset_GearLeverPositionAcceptableCondition = 0;
    unsigned int DQ500Reset_CruiseStalkAcceptableCondition;
    unsigned int DQ500Reset_ClutchCharacteristic_AcceptableConditions = 0;
    unsigned int DQ500Reset_DblClutchAdaptations_AcceptableConditions = 0;
    unsigned int DQ500Reset_BasicSettings_AcceptableConditions = 0;

    unsigned int DQ500_Reset_Mode;
    unsigned int DQ500_Reset_ClutchCharacteristic_Mode;
    unsigned int DQ500_Reset_DblClutchAdapations_Mode;
    unsigned int DQ500_Reset_BasicSettings_Mode;

    
    #pragma endregion

    #pragma region // Variables for Dyno Mode Activation/De-Activation Functions

    unsigned int DynoModeActiation_BrakePedalAcceptableCondition = 0;
    unsigned int DynoModeActivation_EngRPMAcceptableCondition = 0;
    unsigned int DynoModeActivation_GearLeverPositionAcceptableCondition = 0;
    unsigned int DynoModeActivation_CruiseStalkAcceptableCondition = 0;
    unsigned int DynoModeActivation_AcceptableConditions = 0;


   #pragma endregion

    #pragma region // Variables for OEM ECU Diag Functions (DTC Clears, SRI Resets, etc)

      unsigned int LPFP_Activation_MsgSeq;

      unsigned int PT_CAN_LPFP_Activation_Mode;
      unsigned int PT_CAN_LPFP_Activation_Mode_Active;
      unsigned int PT_CAN_LPFP_Activation_Mode_ContinousActive;

      unsigned int PT_CAN_LPFP_Activation_Status_FirstReplyRecieved;
      unsigned int PT_CAN_LPFP_Activation_Status_SecondReplyRecieved;
      unsigned int PT_CAN_LPFP_Activation_Status;

      unsigned int PT_CAN_LPFP_Initiation_FirstReplyRecieved;
      unsigned int PT_CAN_LPFP_Initiation_SecondReplyRecieved;
      unsigned int PT_CAN_LPFP_Initiation_ThirdReplyRecieved;
      unsigned int PT_CAN_LPFP_Initiation_FourthReplyRecieved;


      unsigned int PT_CAN_LPFP_Activation_Continuous_FirstReplyRecieved;
      unsigned int PT_CAN_LPFP_Activation_Continuous_SecondReplyRecieved;


      unsigned int PT_CAN_DTC_Clear_Mode = 0;
      unsigned int PT_CAN_DTC_Clear_Mode_Active = 0;
      unsigned int DTC_Clear_Msg_Sequence = 0;
      unsigned int MQB_SRI_Reset_AcceptableConditions = 0;
      
    #pragma endregion

    #pragma region // Auto-Upshift via PaddleUP Signal Generation 

      
      unsigned int MQB_AutoShift_via_PaddleSignalGeneration_Allowed = 1;
      unsigned int MQB_AutoShift_via_PaddleSignalGeneration_AcceptableCondition = 0;
      unsigned int MQB_AutoUPshift_via_PaddleDOWNSignalGeneration_AcceptableCondition = 0;
      unsigned int PaddleUP_SignalGeneration_Active;
      unsigned int PaddleDOWN_SignalGeneration_Active;


      unsigned int MQB_AutoUPshift_Gear1to2_RPMSetpoint_Int;
      unsigned int MQB_AutoUPshift_Gear2to3_RPMSetpoint_Int;
      unsigned int MQB_AutoUPshift_Gear3to4_RPMSetpoint_Int;
      unsigned int MQB_AutoUPshift_Gear4to5_RPMSetpoint_Int;
      unsigned int MQB_AutoUPshift_Gear5to6_RPMSetpoint_Int;
      unsigned int MQB_AutoUPshift_Gear6to7_RPMSetpoint_Int;


      float MQB_AutoUPshift_Gear1to2_RPMSetpoint_Trigger_Float;
      float MQB_AutoUPshift_Gear2to3_RPMSetpoint_Trigger_Float;
      float MQB_AutoUPshift_Gear3to4_RPMSetpoint_Trigger_Float;
      float MQB_AutoUPshift_Gear4to5_RPMSetpoint_Trigger_Float;
      float MQB_AutoUPshift_Gear5to6_RPMSetpoint_Trigger_Float;
      float MQB_AutoUPshift_Gear6to7_RPMSetpoint_Trigger_Float;

      unsigned int MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Int;
      float MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Float;

      unsigned int MQB_AutoUPshift_Gear1to2_RPMSetpoint_Trigger_Int;
      unsigned int MQB_AutoUPshift_Gear2to3_RPMSetpoint_Trigger_Int;
      unsigned int MQB_AutoUPshift_Gear3to4_RPMSetpoint_Trigger_Int;
      unsigned int MQB_AutoUPshift_Gear4to5_RPMSetpoint_Trigger_Int;
      unsigned int MQB_AutoUPshift_Gear5to6_RPMSetpoint_Trigger_Int;
      unsigned int MQB_AutoUPshift_Gear6to7_RPMSetpoint_Trigger_Int;


      unsigned int MQB_AutoDOWNshift_Gear2to1_RPMSetpoint_Int;
      unsigned int MQB_AutoDOWNshift_Gear3to2_RPMSetpoint_Int;
      unsigned int MQB_AutoDOWNshift_Gear4to3_RPMSetpoint_Int;
      unsigned int MQB_AutoDOWNshift_Gear5to4_RPMSetpoint_Int;
      unsigned int MQB_AutoDOWNshift_Gear6to5_RPMSetpoint_Int;
      unsigned int MQB_AutoDOWNshift_Gear7to6_RPMSetpoint_Int;

      

  // -----------------------------------------------------------------------------------

      unsigned int MQB_AutoShift_RPMSetpoint_UpdateSetpoints_AcceptableConditions;
      unsigned int MQB_AutoShift_UpdateMode_GearShifttoUpdate;
      unsigned int MQB_AutoShift_UpdateMode_GearShifttoUpdateConvertedForDisplay;
      unsigned int MQB_AutoShift_UpdateMode_RPMSetpointConvertedForDisplay;
      unsigned int MQB_AutoShift_UpdateMode_RPMSetpoint_Int_Full;
      unsigned int MQB_AutoShift_UpdateMode_EEPROM_Update_ArmedStatus;
      unsigned int MQB_AutoShift_UpdateMode_OBD_Service22_RequestsEnable;
      unsigned int MQB_AutoShift_UpdateMode_EngineRPM_KalmanFilteredMPC_Gain_Active;

      // unsigned int TFT_KalmanFilteredMPC_FilteredEngRPM;
      // unsigned int TFT_KalmanFilteredMPC_PredictedEngRPM_Extrapolated200ms;
      // unsigned int TFT_KalmanFilteredMPC_PredictedEngRPM_Extrapolated200ms_Filtered;
      

  // -----------------------------------------------------------------------------------

      unsigned int MQB_AutoUPshift_RPMSetpoint_Gear1to2_RPMUpdateModeActive;
      unsigned int MQB_AutoUPshift_RPMSetpoint_Gear2to3_RPMUpdateModeActive;
      unsigned int MQB_AutoUPshift_RPMSetpoint_Gear3to4_RPMUpdateModeActive;
      unsigned int MQB_AutoUPshift_RPMSetpoint_Gear4to5_RPMUpdateModeActive;
      unsigned int MQB_AutoUPshift_RPMSetpoint_Gear5to6_RPMUpdateModeActive;
      unsigned int MQB_AutoUPshift_RPMSetpoint_Gear6to7_RPMUpdateModeActive;

      unsigned int MQB_AutoDOWNshift_RPMSetpoint_Gear2to1_RPMUpdateModeActive;
      unsigned int MQB_AutoDOWNshift_RPMSetpoint_Gear3to2_RPMUpdateModeActive;
      unsigned int MQB_AutoDOWNshift_RPMSetpoint_Gear4to3_RPMUpdateModeActive;
      unsigned int MQB_AutoDOWNshift_RPMSetpoint_Gear5to4_RPMUpdateModeActive;
      unsigned int MQB_AutoDOWNshift_RPMSetpoint_Gear6to5_RPMUpdateModeActive;
      unsigned int MQB_AutoDOWNshift_RPMSetpoint_Gear7to6_RPMUpdateModeActive;

  // -----------------------------------------------------------------------------------

      unsigned int MQB_AutoShift_RPMSetpoints_ALL_EEPROM_UpdateActive;

      unsigned int MQB_AutoUPshift_RPMSetpoint_Gear1to2_EEPROM_UpdateActive;
      unsigned int MQB_AutoUPshift_RPMSetpoint_Gear2to3_EEPROM_UpdateActive;
      unsigned int MQB_AutoUPshift_RPMSetpoint_Gear3to4_EEPROM_UpdateActive;
      unsigned int MQB_AutoUPshift_RPMSetpoint_Gear4to5_EEPROM_UpdateActive;
      unsigned int MQB_AutoUPshift_RPMSetpoint_Gear5to6_EEPROM_UpdateActive;
      unsigned int MQB_AutoUPshift_RPMSetpoint_Gear6to7_EEPROM_UpdateActive;

      unsigned int MQB_AutoDOWNshift_RPMSetpoint_Gear2to1_EEPROM_UpdateActive;
      unsigned int MQB_AutoDOWNshift_RPMSetpoint_Gear3to2_EEPROM_UpdateActive;
      unsigned int MQB_AutoDOWNshift_RPMSetpoint_Gear4to3_EEPROM_UpdateActive;
      unsigned int MQB_AutoDOWNshift_RPMSetpoint_Gear5to4_EEPROM_UpdateActive;
      unsigned int MQB_AutoDOWNshift_RPMSetpoint_Gear6to5_EEPROM_UpdateActive;
      unsigned int MQB_AutoDOWNshift_RPMSetpoint_Gear7to6_EEPROM_UpdateActive;

  // -----------------------------------------------------------------------------------


      unsigned char lsbMQB_AutoUPshift_Gear1to2_RPMSetpoint_Int, msbMQB_AutoUPshift_Gear1to2_RPMSetpoint_Int;
      unsigned char lsbMQB_AutoUPshift_Gear2to3_RPMSetpoint_Int, msbMQB_AutoUPshift_Gear2to3_RPMSetpoint_Int;
      unsigned char lsbMQB_AutoUPshift_Gear3to4_RPMSetpoint_Int, msbMQB_AutoUPshift_Gear3to4_RPMSetpoint_Int;
      unsigned char lsbMQB_AutoUPshift_Gear4to5_RPMSetpoint_Int, msbMQB_AutoUPshift_Gear4to5_RPMSetpoint_Int;
      unsigned char lsbMQB_AutoUPshift_Gear5to6_RPMSetpoint_Int, msbMQB_AutoUPshift_Gear5to6_RPMSetpoint_Int;
      unsigned char lsbMQB_AutoUPshift_Gear6to7_RPMSetpoint_Int, msbMQB_AutoUPshift_Gear6to7_RPMSetpoint_Int;

      unsigned char lsbMQB_AutoUPshift_Gear1to2_RPMSetpoint_Trigger_Int, msbMQB_AutoUPshift_Gear1to2_RPMSetpoint_Trigger_Int;
      unsigned char lsbMQB_AutoUPshift_Gear2to3_RPMSetpoint_Trigger_Int, msbMQB_AutoUPshift_Gear2to3_RPMSetpoint_Trigger_Int;
      unsigned char lsbMQB_AutoUPshift_Gear3to4_RPMSetpoint_Trigger_Int, msbMQB_AutoUPshift_Gear3to4_RPMSetpoint_Trigger_Int;
      unsigned char lsbMQB_AutoUPshift_Gear4to5_RPMSetpoint_Trigger_Int, msbMQB_AutoUPshift_Gear4to5_RPMSetpoint_Trigger_Int;
      unsigned char lsbMQB_AutoUPshift_Gear5to6_RPMSetpoint_Trigger_Int, msbMQB_AutoUPshift_Gear5to6_RPMSetpoint_Trigger_Int;
      unsigned char lsbMQB_AutoUPshift_Gear6to7_RPMSetpoint_Trigger_Int, msbMQB_AutoUPshift_Gear6to7_RPMSetpoint_Trigger_Int;
      
      unsigned char MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Intchar;



      unsigned char lsbMQB_AutoDOWNshift_Gear2to1_RPMSetpoint_Int, msbMQB_AutoDOWNshift_Gear2to1_RPMSetpoint_Int;
      unsigned char lsbMQB_AutoDOWNshift_Gear3to2_RPMSetpoint_Int, msbMQB_AutoDOWNshift_Gear3to2_RPMSetpoint_Int;
      unsigned char lsbMQB_AutoDOWNshift_Gear4to3_RPMSetpoint_Int, msbMQB_AutoDOWNshift_Gear4to3_RPMSetpoint_Int;
      unsigned char lsbMQB_AutoDOWNshift_Gear5to4_RPMSetpoint_Int, msbMQB_AutoDOWNshift_Gear5to4_RPMSetpoint_Int;
      unsigned char lsbMQB_AutoDOWNshift_Gear6to5_RPMSetpoint_Int, msbMQB_AutoDOWNshift_Gear6to5_RPMSetpoint_Int;
      unsigned char lsbMQB_AutoDOWNshift_Gear7to6_RPMSetpoint_Int, msbMQB_AutoDOWNshift_Gear7to6_RPMSetpoint_Int;



    #pragma endregion
      
    #pragma region // LaunchControl EngineRPM and BoostWindow Set and Send Signal in via CANext6/7


      unsigned int MQB_LaunchControl_RampModeActiveStatus;
      unsigned int MQB_LaunchControl_HoldModeActiveStatus;
      unsigned int MQB_LaunchControl_TransitionModeActiveStatus;

      unsigned int MQB_TFT_EngineRPM_Setpoint_Int;
      unsigned int MQB_TFT_EngineRPM_Setpoint_Int_8bit;
      unsigned int MQB_LaunchControl_EngineRPM_Ramp_Setpoint_Int;
      unsigned int MQB_LaunchControl_EngineRPM_Ramp_Setpoint_Int_8bit;
      unsigned int MQB_LaunchControl_EngineRPM_NewHold_Setpoint_Int;
      unsigned int MQB_LaunchControl_EngineRPM_Hold_Setpoint_Int;
      unsigned int MQB_LaunchControl_EngineRPM_Hold_Setpoint_Int_8bit;

      unsigned int MQB_LaunchControl_EngineRPM_Transition_Setpoint_Int;
      unsigned int MQB_LaunchControl_EngineRPM_Transition_Setpoint_Int_8bit;
      float MQB_LaunchControl_EngineRPM_TransitionRateMultiplier_Float; // This is only used at the point of timer PT_CAN_CALculation, where it simply divides the _Int value by 10
      unsigned int MQB_LaunchControl_EngineRPM_TransitionRateMultiplier_Int;

      unsigned int MQB_LaunchControl_EngineRPM_RAMP_Timeout_Int;
      unsigned int MQB_LaunchControl_EngineRPM_PID_BaseOffset_Int;
      float MQB_LaunchControl_EngineRPM_PID_Ki_Term_Float;
      unsigned int MQB_LaunchControl_EngineRPM_PID_Ki_Term_Int;
      
      unsigned int MQB_AutoShift_EngineRPM_KalmanFilteredMPC_Gain_Int;
      float MQB_AutoShift_EngineRPM_KalmanFilteredMPC_Gain_Float;

      unsigned int MQB_LaunchControl_EngineRPM_RAMP_Timeout;

      

      signed int MQB_LaunchControl_EngineRPM_Setpoint_CurrentDelta;
      signed int MQB_LaunchControl_EngineRPM_Setpoint_CurrentDelta_16bitSigned;
      signed int MQB_LaunchControl_EngineRPM_Setpoint_CurrentDelta_8bit;

      signed int MQB_IgnTimingCorrection;
      signed int MQB_IgnTimingCorrection_8bitSigned;

      unsigned int MQB_LaunchControl_BoostWindow_EntryThresholdPressure_Int;
      unsigned int MQB_LaunchControl_BoostWindow_minMaintainPressure_Int;
      unsigned int MQB_LaunchControl_BoostVariableSelection_Int;


      unsigned int MQB_LaunchControl_EngineRPM_Setpoint_Allowed = 1;
      unsigned int MQB_LaunchControl_EngineRPM_Setpoint_AcceptableCondition = 0;


      unsigned int MQB_LaunchControl_BoostWindow_Setpoint_Allowed = 1;
      unsigned int MQB_LaunchControl_BoostWindow_EntryThresholdPressure_Achieved;
      unsigned int MQB_LaunchControl_BoostWindow_EntryThresholdPressure_Hysterisis;
      unsigned int MQB_LaunchControl_BoostWindow_Maintained_Status;
      unsigned int MQB_LaunchControl_CombinedSignalActive;
      unsigned int MQB_LaunchControl_CombinedSignalActive_8bit;


      unsigned int MQB_LaunchControl_UpdateMode_VariabletoUpdate;
      unsigned int MQB_LaunchControl_UpdateMode_VariabletoUpdateConvertedForDisplay;
      unsigned int MQB_LaunchControl_UpdateMode_RPMSetpointConvertedForDisplay;
      unsigned int MQB_LaunchControl_UpdateMode_BoostWindow_EntryThresholdPressure_ConvertedForDisplay;
      unsigned int MQB_LaunchControl_UpdateMode_BoostWindow_minMaintainPressure_ConvertedForDisplay;
      unsigned int MQB_LaunchControl_UpdateMode_EEPROM_Update_ArmedStatus;
      unsigned int MQB_LaunchControl_UpdateMode_OBD_Service22_RequestsEnable;

      unsigned int MQB_LaunchControl_Variable_Update_AcceptableCondition = 0;
      unsigned int MQB_LaunchControl_EngineRPM_Setpoint_Update_AcceptableCondition = 0;
      unsigned int MQB_LaunchControl_BoostWindow_EntryThresholdPressure_Update_AcceptableCondition = 0;
      unsigned int MQB_LaunchControl_BoostWindow_minMaintainPressure_Update_AcceptableCondition = 0;

    // -----------------------------------------------------------------------------------

      unsigned int MQB_LaunchControl_UpdateMode_EngineRPM_Setpoint;
      unsigned int MQB_LaunchControl_UpdateMode_BoostWindow_EntryThresholdPressure;
      unsigned int MQB_LaunchControl_UpdateMode_BoostWindow_minMaintainPressure;

    // -----------------------------------------------------------------------------------

      unsigned int MQB_LaunchControl_UpdateMode_EngineRPM_Ramp_Setpoint_Active;
      unsigned int MQB_LaunchControl_UpdateMode_EngineRPM_Hold_Setpoint_Active;
      unsigned int MQB_LaunchControl_UpdateMode_EngineRPM_TransitionRateMultiplier_Active;

      unsigned int MQB_LaunchControl_UpdateMode_EngineRPM_RAMP_Timeout_Active;
      unsigned int MQB_LaunchControl_UpdateMode_EngineRPM_PID_BaseOffset_Active;
      unsigned int MQB_LaunchControl_UpdateMode_EngineRPM_PID_Ki_Term_Active;
      
      unsigned int MQB_LaunchControl_UpdateMode_BoostVariableSelection_Active;
      unsigned int MQB_LaunchControl_UpdateMode_BoostWindow_EntryThresholdPressure_Active;
      unsigned int MQB_LaunchControl_UpdateMode_BoostWindow_minMaintainPressure_Active;

    // -----------------------------------------------------------------------------------

      unsigned int MQB_LaunchControl_AllSetpoints_EEPROM_UpdateActive;
      unsigned int MQB_LaunchControl_BoostWindow_EntryThresholdPressure_EEPROM_UpdateActive;
      unsigned int MQB_LaunchControl_BoostWindow_minMaintainPressure_EEPROM_UpdateActive;

    // -----------------------------------------------------------------------------------


    #pragma endregion

    #pragma region // BurnoutMode EngineRPM Setpoints - Send Signal in via CANextXX

      unsigned int MQB_BurnoutMode_EngineRPM_Setpoint_Int;
      unsigned int MQB_BurnoutMode_EngineRPM_Setpoint_Int_8bit;

      unsigned int MQB_BurnoutMode_EngineRPM_Setpoint_Allowed = 1;
      unsigned int MQB_BurnoutMode_EngineRPM_Setpoint_AcceptableCondition = 0;
      unsigned int MQB_BurnoutMode_ActiveStatus;

      unsigned int MQB_BurnoutMode_UpdateMode_EngineRPM_Setpoint;
      unsigned int MQB_BurnoutMode_UpdateMode_EngineRPM_Setpoint_Active;
      unsigned int MQB_BurnoutMode_EngineRPM_Setpoint_EEPROM_UpdateActive;

      #pragma endregion

    #pragma region // Parachute ArmSpeed Setpoints - Send Signal in via CANextXX


      unsigned int MQB_Parachute_ArmSpeed_Setpoint_Int;
      unsigned int MQB_Parachute_ArmSpeed_Setpoint_Int_8bit;

      

      // unsigned int MQB_Parachute_ArmSpeed_Setpoint_Allowed = 1;
      // unsigned int MQB_Parachute_ArmSpeed_Setpoint_AcceptableCondition = 0;
      unsigned int MQB_Parachute_ArmSpeed_Setpoint_ActiveStatus;

      unsigned int MQB_Parachute_UpdateMode_ArmSpeed_Setpoint;
      unsigned int MQB_Parachute_UpdateMode_ArmSpeed_Setpoint_Active;
      unsigned int MQB_Parachute_ArmSpeed_Setpoint_EEPROM_UpdateActive;



      #pragma endregion

    #pragma region // Fuel Overrun Override Mode via CANext6 CombinedSignal (Activated by "Back" button on MFSW)

      unsigned int MQB_FuelOverrun_OverrideModeActive_Status;
      unsigned int MQB_FuelOverrun_OverrideMode_Allowed = 1;

    #pragma endregion

    #pragma region // Bit-shifted variables for LaunchControl, BoostWindow, BurnoutMode Setpoints, etc

      unsigned char lsbMQB_LaunchControl_EngineRPM_Ramp_Setpoint_Int, msbMQB_LaunchControl_EngineRPM_Ramp_Setpoint_Int;
      unsigned char lsbMQB_LaunchControl_EngineRPM_Ramp_Setpoint_Int_8bit;
      
      unsigned char lsbMQB_LaunchControl_EngineRPM_Hold_Setpoint_Int, msbMQB_LaunchControl_EngineRPM_Hold_Setpoint_Int;
      unsigned char lsbMQB_LaunchControl_EngineRPM_Hold_Setpoint_Int_8bit;
      
      unsigned char lsbMQB_LaunchControl_EngineRPM_Transition_Setpoint_Int, msbMQB_LaunchControl_EngineRPM_Transition_Setpoint_Int;
      unsigned char lsbMQB_LaunchControl_EngineRPM_Transition_Setpoint_Int_8bit;

      unsigned char lsbMQB_LaunchControl_EngineRPM_TransitionRateMultiplier_Int, msbMQB_LaunchControl_EngineRPM_TransitionRateMultiplier_Int;
      // unsigned char lsbMQB_LaunchControl_EngineRPM_Transition_Setpoint_Int_8bit;


      unsigned char lsbMQB_LaunchControl_EngineRPM_RAMP_Timeout_Int, msbMQB_LaunchControl_EngineRPM_RAMP_Timeout_Int;

      unsigned char lsbMQB_LaunchControl_EngineRPM_PID_BaseOffset_Int, msbMQB_LaunchControl_EngineRPM_PID_BaseOffset_Int;

      unsigned char lsbMQB_LaunchControl_EngineRPM_PID_Ki_Term_Int, msbMQB_LaunchControl_EngineRPM_PID_Ki_Term_Int;

      unsigned char lsbMQB_AutoShift_EngineRPM_KalmanFilteredMPC_Gain_Int, msbMQB_AutoShift_EngineRPM_KalmanFilteredMPC_Gain_Int;


// -------------------------------------------------------------------------------------------------------------------------------------

      signed char lsbMQB_LaunchControl_EngineRPM_Setpoint_CurrentDelta, msbMQB_LaunchControl_EngineRPM_Setpoint_CurrentDelta;
      signed char lsbMQB_LaunchControl_EngineRPM_Setpoint_CurrentDelta_8bit;

// -------------------------------------------------------------------------------------------------------------------------------------
    
      unsigned char lsbMQB_TFT_EngineRPM_Setpoint_Int, msbMQB_TFT_EngineRPM_Setpoint_Int;
      unsigned char lsbMQB_TFT_EngineRPM_Setpoint_Int_8bit;

// -------------------------------------------------------------------------------------------------------------------------------------

      unsigned char lsbMQB_LaunchControl_BoostWindow_EntryThresholdPressure_Int, msbMQB_LaunchControl_BoostWindow_EntryThresholdPressure_Int;
      unsigned char lsbMQB_LaunchControl_BoostWindow_EntryThresholdPressure_Int_8bit;

      unsigned char lsbMQB_LaunchControl_BoostWindow_minMaintainPressure_Int, msbMQB_LaunchControl_BoostWindow_minMaintainPressure_Int;
      unsigned char lsbMQB_LaunchControl_BoostWindow_minMaintainPressure_Int_8bit;

      unsigned char lsbMQB_LaunchControl_BoostVariableSelection_Int, msbMQB_LaunchControl_BoostVariableSelection_Int;
      unsigned char lsbMQB_LaunchControl_BoostVariableSelection_Int_8bit;

// -------------------------------------------------------------------------------------------------------------------------------------

      unsigned char lsbMQB_LaunchControl_BoostWindow_Maintained_Status, msbMQB_LaunchControl_BoostWindow_Maintained_Status;
      unsigned char lsbMQB_LaunchControl_BoostWindow_Maintained_Status_8bit;

// -------------------------------------------------------------------------------------------------------------------------------------

      unsigned char lsbMQB_TractionControl_TgtSlipRate_Int, msbMQB_TractionControl_TgtSlipRate_Int;
      unsigned char lsbMQB_TractionControl_CorrectionAggression_Int, msbMQB_TractionControl_CorrectionAggression_Int;
      unsigned char lsbMQB_TractionControl_CorrectionAggression_Int_8bit;

// -------------------------------------------------------------------------------------------------------------------------------------

      unsigned char lsbMQB_BurnoutMode_EngineRPM_Setpoint_Int, msbMQB_BurnoutMode_EngineRPM_Setpoint_Int;
      unsigned char lsbMQB_BurnoutMode_EngineRPM_Setpoint_Int_8bit;

// -------------------------------------------------------------------------------------------------------------------------------------

      unsigned char lsbMQB_LaunchControl_CombinedSignalActive, msbMQB_LaunchControl_CombinedSignalActive;
      unsigned char lsbMQB_LaunchControl_CombinedSignalActive_8bit;

// -------------------------------------------------------------------------------------------------------------------------------------

      unsigned char lsbMQB_Parachute_ArmSpeed_Setpoint_Int, msbMQB_Parachute_ArmSpeed_Setpoint_Int;
      unsigned char lsbMQB_Parachute_ArmSpeed_Setpoint_Int_8bit;
      unsigned char lsbMQB_Parachute_PreArmedStatus;



    #pragma endregion
    


#pragma endregion 


#pragma region // General Keep-Awake canTx Functions for ECUs on PT_CAN - Copied from OBD2 versions

void canTx_PT_CAN_GeneralKeepAwake_0x700_2_3E_80() { // function to send keep-awake signals into PT_CAN
  CAN_message_t msgTx, msgRx;
  msgTx.buf[0] = 0x2;     // Data Length 2byte
  msgTx.buf[1] = 0x3E;     // 
  msgTx.buf[2] = 0x80;     // 
  msgTx.buf[3] = 0x55;        // Padding
  msgTx.buf[4] = 0x55;
  msgTx.buf[5] = 0x55;
  msgTx.buf[6] = 0x55;
  msgTx.buf[7] = 0x55;
  msgTx.len = 8;            // number of bytes in request
  msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
  //    msgTx.flags.extended = 1; // 29 bit header, not 11 bit 
  msgTx.flags.remote = 0;

  msgTx.id = 0x700; // request header for OBD           // Physical Addressing Request to Engine ECU
  //  msgTx.id = 0x18DB33F1; // request header for OBD Functional Addressing request to all emission related ECUs.
  TFTCAN3.write(msgTx);

  // Serial.println();
  // Serial.print("canTx_PT_CAN_GeneralKeepAwake_0x700_2_3E_80 KeepAwake Sent  ");  Serial.print("\t");
  // Serial.println();

}

void canTx_PT_CAN_22_GeneralKeepAwake_0x713_3_22_F1_99_55() { // function to send keep-awake signals into PT_CAN
  CAN_message_t msgTx, msgRx;
  msgTx.buf[0] = 0x3;     // Data Length 2byte
  msgTx.buf[1] = 0x22;     // 
  msgTx.buf[2] = 0xF1;     // 
  msgTx.buf[3] = 0x99;        // Padding
  msgTx.buf[4] = 0x55;
  msgTx.buf[5] = 0x55;
  msgTx.buf[6] = 0x55;
  msgTx.buf[7] = 0x55;
  msgTx.len = 8;            // number of bytes in request
  msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
  //    msgTx.flags.extended = 1; // 29 bit header, not 11 bit 
  msgTx.flags.remote = 0;

  msgTx.id = 0x713; // request header for OBD           // Physical Addressing Request to Engine ECU
  //  msgTx.id = 0x18DB33F1; // request header for OBD Functional Addressing request to all emission related ECUs.
  TFTCAN2.write(msgTx);

  Serial.println("");
  Serial.print("canTx_PT_CAN_22_GeneralKeepAwake_0x713_3_22_F1_99_55 Sent");
  Serial.println("");
}

void canTx_PT_CAN_GeneralKeepAwake_0x700_2_10_3() { // function to send keep-awake signals into PT_CAN
  CAN_message_t msgTx, msgRx;
  msgTx.buf[0] = 0x2;     // Data Length 2byte
  msgTx.buf[1] = 0x10;     // 
  msgTx.buf[2] = 0x03;     // 
  msgTx.buf[3] = 0x55;        // Padding
  msgTx.buf[4] = 0x55;
  msgTx.buf[5] = 0x55;
  msgTx.buf[6] = 0x55;
  msgTx.buf[7] = 0x55;
  msgTx.len = 8;            // number of bytes in request
  msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
  //    msgTx.flags.extended = 1; // 29 bit header, not 11 bit 
  msgTx.flags.remote = 0;

  msgTx.id = 0x700; // request header for OBD           // Physical Addressing Request to Engine ECU
  //  msgTx.id = 0x18DB33F1; // request header for OBD Functional Addressing request to all emission related ECUs.
  TFTCAN2.write(msgTx);

  Serial.println();
  Serial.print("canTx_PT_CAN_GeneralKeepAwake_0x700_2_10_3 Sent  ");  Serial.print("\t");
  Serial.println();



}



#pragma endregion 

#pragma region // canTX Functions - OBD - ECU 0x7E1/0x7E8 - OBD DTC Clear Codes - Copied from OBD2 versions

  void canSniffService22_PT_CAN_0x700(const CAN_message_t& PT_CAN_msg0x700) { // Sniffing of 0x700

    //  Serial.println(" ");
    //  Serial.print("MB: ");
    //  Serial.print(PT_CAN_msg0x700.mb);
    //  Serial.print("  ID: 0x");
    //  Serial.print(PT_CAN_msg0x700.id, HEX);
    //  Serial.print("  EXT: ");
    //  Serial.print(PT_CAN_msg0x700.flags.extended);
    //  Serial.print("  LEN: ");
    //  Serial.print(PT_CAN_msg0x700.len);
    //  Serial.print(" DATA: ");
    for (uint8_t i = 0; i < 8; i++)
    {
      //  Serial.print(PT_CAN_msg0x700.buf[i], HEX);
      //  Serial.print(" ");
    }

    //  Serial.println("");


  #pragma region // Recognize 0x700 Response Frames, coordinate buffered messages

    if (PT_CAN_msg0x700.buf[0] == 0x03 && PT_CAN_msg0x700.buf[1] == 0x6e) {    ////////// Positive Response message  and First Frame.
      PT_CAN_ECU_0x700_CF_Max = 1;
      PT_CAN_f_sid22_0x700_multi = 1;   // 

      if (PT_CAN_msg0x700.buf[2] == 0xF1) {   // 

        PT_CAN_msg0x700_buf0 = PT_CAN_msg0x700.buf[0];
        PT_CAN_msg0x700_buf1 = PT_CAN_msg0x700.buf[1];
        PT_CAN_msg0x700_buf2 = PT_CAN_msg0x700.buf[2];
        PT_CAN_msg0x700_buf3 = PT_CAN_msg0x700.buf[3];
        PT_CAN_msg0x700_buf4 = PT_CAN_msg0x700.buf[4];
        PT_CAN_msg0x700_buf5 = PT_CAN_msg0x700.buf[5];
        PT_CAN_msg0x700_buf6 = PT_CAN_msg0x700.buf[6];
        PT_CAN_msg0x700_buf7 = PT_CAN_msg0x700.buf[7];
        PT_CAN_ECU_0x700_STAT_FC_22 = 1;

        // Serial.print(PT_CAN_msg0x700.buf[0], HEX);     Serial.print(" ");
        // Serial.print(PT_CAN_msg0x700.buf[1], HEX);     Serial.print(" ");
        // Serial.print(PT_CAN_msg0x700.buf[2], HEX);     Serial.print(" ");
        // Serial.print(PT_CAN_msg0x700.buf[3], HEX);     Serial.print(" ");
        // Serial.print(PT_CAN_msg0x700.buf[4], HEX);     Serial.print(" ");
        // Serial.print(PT_CAN_msg0x700.buf[5], HEX);     Serial.print(" ");
        // Serial.print(PT_CAN_msg0x700.buf[6], HEX);     Serial.print(" ");
        // Serial.print(PT_CAN_msg0x700.buf[7], HEX);     Serial.print(" ");

      }
    }
    else if (PT_CAN_f_sid22_0x700_multi == 1 && PT_CAN_msg0x700.buf[0] > 20 && PT_CAN_ECU_0x700_STAT_FC_22 == 2) {     // Means CF: Consecutive Frame and Buf[0] is sequence number
      PT_CAN_ECU_0x700_seq_Num = PT_CAN_msg0x700.buf[0]; //
      //  Serial.print("Starting ElseIF  ");

      // Serial.print(PT_CAN_msg0x700.buf[0], HEX);     Serial.print(" ");
      // Serial.print(PT_CAN_msg0x700.buf[1], HEX);     Serial.print(" ");
      // Serial.print(PT_CAN_msg0x700.buf[2], HEX);     Serial.print(" ");
      // Serial.print(PT_CAN_msg0x700.buf[3], HEX);     Serial.print(" ");
      // Serial.print(PT_CAN_msg0x700.buf[4], HEX);     Serial.print(" ");
      // Serial.print(PT_CAN_msg0x700.buf[5], HEX);     Serial.print(" ");
      // Serial.print(PT_CAN_msg0x700.buf[6], HEX);     Serial.print(" ");
      // Serial.print(PT_CAN_msg0x700.buf[7], HEX);     Serial.print(" ");

      if (PT_CAN_ECU_0x700_seq_Num == 0x21) {   // Sequence Number is 1.
        PT_CAN_msg0x700_CF21_buf1 = PT_CAN_msg0x700.buf[1];
        PT_CAN_msg0x700_CF21_buf2 = PT_CAN_msg0x700.buf[2];
        PT_CAN_msg0x700_CF21_buf3 = PT_CAN_msg0x700.buf[3];
        PT_CAN_msg0x700_CF21_buf4 = PT_CAN_msg0x700.buf[4];
        PT_CAN_msg0x700_CF21_buf5 = PT_CAN_msg0x700.buf[5];
        PT_CAN_msg0x700_CF21_buf6 = PT_CAN_msg0x700.buf[6];
        PT_CAN_msg0x700_CF21_buf7 = PT_CAN_msg0x700.buf[7]; // last byte not used
        // PT_CAN_CF_Num = 1;
        PT_CAN_ECU_0x700_CF_Num = 1;
        // Serial.print("CFNum = 1");

      }
      if (PT_CAN_ECU_0x700_CF_Num == PT_CAN_ECU_0x700_CF_Max) {
        if (PT_CAN_ECU_0x700_STAT_FC_22 == 2) {
          PT_CAN_ECU_0x700_STAT_FC_22 = 0;
          // Serial.print("CFMax: ");

          PT_CAN_ECU_0x700_CF_Num = 0;
          PT_CAN_f_sid22_0x700_multi = 0;
          //  Serial.print("CFMax: ");
          //  Serial.print(CFNum);

        }


        // Serial.println();

      }
    }

  #pragma endregion

  }

  void canSniffService22_PT_CAN_0x7E8(const CAN_message_t& PT_CAN_msg0x7E8) { // Sniffing of 0x7E8 (0x7E8 is a response to request sent to 0x700)

    //  Serial.println(" ");
    //  Serial.print("MB: ");
    //  Serial.print(PT_CAN_msg0x7E8.mb);
    //  Serial.print("  ID: 0x");
    //  Serial.print(PT_CAN_msg0x7E8.id, HEX);
    //  Serial.print("  EXT: ");
    //  Serial.print(PT_CAN_msg0x7E8.flags.extended);
    //  Serial.print("  LEN: ");
    //  Serial.print(PT_CAN_msg0x7E8.len);
    //  Serial.print(" DATA: ");
    for (uint8_t i = 0; i < 8; i++)
    {
      //  Serial.print(PT_CAN_msg0x7E8.buf[i], HEX);
      //  Serial.print(" ");
    }

    //  Serial.println("");


  #pragma region // Recognize 0x7E8 Response Frames, coordinate buffered messages

    if (PT_CAN_msg0x7E8.buf[0] == 0x03 && PT_CAN_msg0x7E8.buf[1] == 0x6e) {    ////////// Positive Response message  and First Frame.
      PT_CAN_ECU_0x7E8_CF_Max = 1;
      PT_CAN_f_sid22_0x7E8_multi = 1;   // 

      if (PT_CAN_msg0x7E8.buf[2] == 0xF1) {   // 

        PT_CAN_msg0x7E8_buf0 = PT_CAN_msg0x7E8.buf[0];
        PT_CAN_msg0x7E8_buf1 = PT_CAN_msg0x7E8.buf[1];
        PT_CAN_msg0x7E8_buf2 = PT_CAN_msg0x7E8.buf[2];
        PT_CAN_msg0x7E8_buf3 = PT_CAN_msg0x7E8.buf[3];
        PT_CAN_msg0x7E8_buf4 = PT_CAN_msg0x7E8.buf[4];
        PT_CAN_msg0x7E8_buf5 = PT_CAN_msg0x7E8.buf[5];
        PT_CAN_msg0x7E8_buf6 = PT_CAN_msg0x7E8.buf[6];
        PT_CAN_msg0x7E8_buf7 = PT_CAN_msg0x7E8.buf[7];
        PT_CAN_ECU_0x7E8_STAT_FC_22 = 1;

        // Serial.print(PT_CAN_msg0x7E8.buf[0], HEX);     Serial.print(" ");
        // Serial.print(PT_CAN_msg0x7E8.buf[1], HEX);     Serial.print(" ");
        // Serial.print(PT_CAN_msg0x7E8.buf[2], HEX);     Serial.print(" ");
        // Serial.print(PT_CAN_msg0x7E8.buf[3], HEX);     Serial.print(" ");
        // Serial.print(PT_CAN_msg0x7E8.buf[4], HEX);     Serial.print(" ");
        // Serial.print(PT_CAN_msg0x7E8.buf[5], HEX);     Serial.print(" ");
        // Serial.print(PT_CAN_msg0x7E8.buf[6], HEX);     Serial.print(" ");
        // Serial.print(PT_CAN_msg0x7E8.buf[7], HEX);     Serial.print(" ");

      }
    }
    else if (PT_CAN_f_sid22_0x7E8_multi == 1 && PT_CAN_msg0x7E8.buf[0] > 20 && PT_CAN_ECU_0x7E8_STAT_FC_22 == 2) {     // Means CF: Consecutive Frame and Buf[0] is sequence number
      PT_CAN_ECU_0x7E8_seq_Num = PT_CAN_msg0x7E8.buf[0]; //
      //  Serial.print("Starting ElseIF  ");

      // Serial.print(PT_CAN_msg0x7E8.buf[0], HEX);     Serial.print(" ");
      // Serial.print(PT_CAN_msg0x7E8.buf[1], HEX);     Serial.print(" ");
      // Serial.print(PT_CAN_msg0x7E8.buf[2], HEX);     Serial.print(" ");
      // Serial.print(PT_CAN_msg0x7E8.buf[3], HEX);     Serial.print(" ");
      // Serial.print(PT_CAN_msg0x7E8.buf[4], HEX);     Serial.print(" ");
      // Serial.print(PT_CAN_msg0x7E8.buf[5], HEX);     Serial.print(" ");
      // Serial.print(PT_CAN_msg0x7E8.buf[6], HEX);     Serial.print(" ");
      // Serial.print(PT_CAN_msg0x7E8.buf[7], HEX);     Serial.print(" ");

      if (PT_CAN_ECU_0x7E8_seq_Num == 0x21) {   // Sequence Number is 1.
        PT_CAN_msg0x7E8_CF21_buf1 = PT_CAN_msg0x7E8.buf[1];
        PT_CAN_msg0x7E8_CF21_buf2 = PT_CAN_msg0x7E8.buf[2];
        PT_CAN_msg0x7E8_CF21_buf3 = PT_CAN_msg0x7E8.buf[3];
        PT_CAN_msg0x7E8_CF21_buf4 = PT_CAN_msg0x7E8.buf[4];
        PT_CAN_msg0x7E8_CF21_buf5 = PT_CAN_msg0x7E8.buf[5];
        PT_CAN_msg0x7E8_CF21_buf6 = PT_CAN_msg0x7E8.buf[6];
        PT_CAN_msg0x7E8_CF21_buf7 = PT_CAN_msg0x7E8.buf[7]; // last byte not used
        // PT_CAN_CF_Num = 1;
        PT_CAN_ECU_0x7E8_CF_Num = 1;
        // Serial.print("CFNum = 1");

      }
      if (PT_CAN_ECU_0x7E8_CF_Num == PT_CAN_ECU_0x7E8_CF_Max) {
        if (PT_CAN_ECU_0x7E8_STAT_FC_22 == 2) {
          PT_CAN_ECU_0x7E8_STAT_FC_22 = 0;
          // Serial.print("CFMax: ");

          PT_CAN_ECU_0x7E8_CF_Num = 0;
          PT_CAN_f_sid22_0x7E8_multi = 0;
          //  Serial.print("CFMax: ");
          //  Serial.print(CFNum);

        }


        // Serial.println();

      }
    }

  #pragma endregion

  }

  void canTx_PT_CAN_22_0x700_PT_CAN_DTC_Clear_ALL_4_14_FF() { // function to send message to clear DTCs to ALL modules via OBD 
    CAN_message_t msgTx, msgRx;
    msgTx.buf[0] = 0x4;     // Data Length 2byte
    msgTx.buf[1] = 0x14;     // 
    msgTx.buf[2] = 0xFF;     // 
    msgTx.buf[3] = 0xFF;        // Padding
    msgTx.buf[4] = 0xFF;
    msgTx.buf[5] = 0xBF;
    msgTx.buf[6] = 0xBF;
    msgTx.buf[7] = 0xBF;
    msgTx.len = 8;            // number of bytes in request
    msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
    //    msgTx.flags.extended = 1; // 29 bit header, not 11 bit 
    msgTx.flags.remote = 0;

    msgTx.id = 0x700; // request header for OBD           // Physical Addressing Request to Engine ECU
    //  msgTx.id = 0x18DB33F1; // request header for OBD Functional Addressing request to all emission related ECUs.
    TFTCAN3.write(msgTx);
    Serial.println();
    Serial.print("canTx_PT_CAN_22_0x700_PT_CAN_DTC_Clear_ALL_4_14_FF Sent  ");  Serial.print("\t");
    Serial.println();


  }

  void canTx_PT_CAN_22_0x700_PT_CAN_DTC_Clear_All_1_4_00() { // function to send message to clear DTCs via OBD 
    CAN_message_t msgTx, msgRx;
    msgTx.buf[0] = 0x01;     // Data Length 2byte
    msgTx.buf[1] = 0x04;     // 
    msgTx.buf[2] = 0x00;     // 
    msgTx.buf[3] = 0x00;        // Padding
    msgTx.buf[4] = 0x00;
    msgTx.buf[5] = 0x00;
    msgTx.buf[6] = 0x00;
    msgTx.buf[7] = 0x00;
    msgTx.len = 8;            // number of bytes in request
    msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
    //    msgTx.flags.extended = 1; // 29 bit header, not 11 bit 
    msgTx.flags.remote = 0;

    msgTx.id = 0x700; // request header for OBD           // Physical Addressing Request to Engine ECU
    //  msgTx.id = 0x18DB33F1; // request header for OBD Functional Addressing request to all emission related ECUs.
    TFTCAN3.write(msgTx);
    Serial.print("canTx_PT_CAN_22_0x700_PT_CAN_DTC_Clear_All_1_4_00 Sent");
    loopDelayPT_CAN_DTC_Clear = 0;

  }

  void canTx_PT_CAN_22_0x700_PT_CAN_FlowControl() { //  function to manage flowcontrol buffers
    CAN_message_t msgTx, msgRx;
    msgTx.buf[0] = 0x30;     // Data Length 2byte
    msgTx.buf[1] = 0x0;     // 
    msgTx.buf[2] = 0x01;     // 
    msgTx.buf[3] = 0x55;        // Padding
    msgTx.buf[4] = 0x55;
    msgTx.buf[5] = 0x55;
    msgTx.buf[6] = 0x55;
    msgTx.buf[7] = 0x55;
    msgTx.len = 8;            // number of bytes in request
    // msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
    msgTx.flags.extended = 0; // 29 bit header, not 11 bit 
    msgTx.flags.remote = 0;


    msgTx.id = 0x700; // request header for OBD           // Physical Addressing Request to Engine ECU
    // msgTx.id = 0x18DA11F1; // request header for OBD Functional Addressing request to all emission related ECUs.
  //  msgTx.id = 0x18DB33F1; // request header for OBD Functional Addressing request to all emission related ECUs.
    TFTCAN3.write(msgTx);
    PT_CAN_ECU_0x7E8_STAT_FC_22 = 2;  // Flow Control sent
    //  Serial.print("canTx_PT_CAN_22_0x700_PT_CAN_FlowControl REQ sent"); 

  }

#pragma endregion



#pragma region // canRx Services - General/Passive CAN Sniffing of RS3 PT-CAN Frames

// 0x7EE byte 0 is map slot number when RealDash is active
// 0x32C bytes 4 and 5 are Engine RPM rev limit (not linked to shift light or combi rev limit)
// 0x670 byte 7 is Shift Light RPM  - 3000rpm results in 0x3c or decimal 60 




  void canSniff0x40(const CAN_message_t &PT_CAN_msg0x40) // function to sniff for Ign Status
{ // function to sniff OEM CAN 0x081 for ignition status

      //  Serial.print("MB "); Serial.print(PT_CAN_msg0x40.mb);
      //  Serial.print(" ID: "); Serial.print(PT_CAN_msg0x40.id, HEX);
      //  Serial.print(" Buffer: ");

  for (uint8_t i = 0; i < PT_CAN_msg0x40.len; i++)
  {
              //  Serial.print(PT_CAN_msg0x40.buf[i], DEC); Serial.print(" ");
                    digitalToggle(LED_PIN_CANRecieve);
                    IgnitionStatusTimer_TimeSinceLastMessage = 0;
                    

  }
    //  Serial.println("  ");

  // word IgnitionStatusCAN(PT_CAN_msg0x40.buf[6] + (PT_CAN_msg0x40.buf[7] * 256));
  //  IgnitionStatus = IgnitionStatusCAN;

  word IgnitionStatusTimerCAN(PT_CAN_msg0x40.buf[1]);
  IgnitionStatusTimer = IgnitionStatusTimerCAN;

  IgnitionStatusTimer_TimeSinceLastMessage = 0;

  digitalToggle(LED_PIN_CANRecieve);

// if (IgnitionStatus != 65504)
//   {
//      IgnitionAwake = 1;
//      ShutdownTimer = 25000;
//   }
  // if (IgnitionStatus == 65504)
  // {
  //   IgnitionAwake = 0;
  //   ShutdownTimer = 0;
     
  // }

  // else if (IgnitionStatus != 65504 || IgnitionStatus == 0)
  // {
  //    IgnitionAwake = 1;
  //    ShutdownTimer = 25000;
  // }


      //  Serial.print(" IgnitionStatusCAN:  ");   Serial.print(IgnitionStatusCAN);  Serial.print(" \t ");
      //  Serial.print(" IgnitionStatus:  ");   Serial.print(IgnitionStatus);  Serial.print(" \t ");
      //  Serial.print(" IgnitionAwake:  ");   Serial.print(IgnitionAwake);  Serial.println(" \t ");

}

  void canSniff0x0A8(const CAN_message_t &PT_CAN_msg0x0A8) { // function to sniff for Engine RPM
    //  Serial.print("MB "); Serial.print(PT_CAN_msg0x0A8.mb);
    //  Serial.print(" ID: "); Serial.print(PT_CAN_msg0x0A8.id, HEX);
    //  Serial.print(" Buffer: ");
    for (uint8_t i = 0; i < PT_CAN_msg0x0A8.len; i++)
    {
        //  Serial.print(PT_CAN_msg0x0A8.buf[i], HEX); Serial.print(" ");
    }
          // Serial.println("  ");

    word EngineRPM_0xA8CAN = ((PT_CAN_msg0x0A8.buf[7]) * 256 ) + (PT_CAN_msg0x0A8.buf[6]);
    EngineRPM_0xA8 = (EngineRPM_0xA8CAN / 4);

  // Serial.print("EngineRPM_0xA8: ");
  // Serial.print(EngineRPM_0xA8);
  // Serial.println();
  }

  void canSniff0xAD(const CAN_message_t &PT_CAN_msg0xAD) { // function to sniff for Gear Lever Position

    //  Serial.print("MB "); Serial.print(PT_CAN_msg0xAD.mb);
    //  Serial.print(" ID: "); Serial.print(PT_CAN_msg0xAD.id, HEX);
    //  Serial.print(" Buffer: ");
    for (uint8_t i = 0; i < PT_CAN_msg0xAD.len; i++)
    {
        //  Serial.print(PT_CAN_msg0xAD.buf[i], HEX); Serial.print(" ");
    }
          // Serial.println("  ");

    word GearLeverPosition_0xADCAN = (PT_CAN_msg0xAD.buf[5]);
    GearLeverPosition_0xAD = GearLeverPosition_0xADCAN;


    PT_CAN_msg0xAD_buf0_Raw = PT_CAN_msg0xAD.buf[0];
    PT_CAN_msg0xAD_buf1_Raw = PT_CAN_msg0xAD.buf[1];
    PT_CAN_msg0xAD_buf2_Raw = PT_CAN_msg0xAD.buf[2];
    PT_CAN_msg0xAD_buf3_Raw = PT_CAN_msg0xAD.buf[3];
    PT_CAN_msg0xAD_buf4_Raw = PT_CAN_msg0xAD.buf[4];
    PT_CAN_msg0xAD_buf5_Raw = PT_CAN_msg0xAD.buf[5];
    PT_CAN_msg0xAD_buf6_Raw = PT_CAN_msg0xAD.buf[6];
    PT_CAN_msg0xAD_buf7_Raw = PT_CAN_msg0xAD.buf[7];
    

  // Serial.print("GearLeverPosition_0xAD: ");
  // Serial.print(GearLeverPosition_0xAD);
  // Serial.println();


  }

  void canSniff0xB2(const CAN_message_t &PT_CAN_msg0xB2) { // function to sniff for Wheel Speeds 


    // if (PT_CAN_msg0xB2.buf[0] == 0x1D || PT_CAN_msg0xB2.buf[0] == 0x1E || PT_CAN_msg0xB2.buf[0] == 0x1F ) {

    //  Serial.print("MB "); Serial.print(PT_CAN_msg0xB2.mb);
    //  Serial.print(" ID: "); Serial.print(PT_CAN_msg0xB2.id, HEX);
    //  Serial.print(" Buffer: ");
    for (uint8_t i = 0; i < PT_CAN_msg0xB2.len; i++)
    {
        //  Serial.print(PT_CAN_msg0xB2.buf[i], HEX); Serial.print(" ");
    }
          // Serial.println("  ");

          PT_CAN_msg0xB2x1buf0 = PT_CAN_msg0xB2.buf[0];
          PT_CAN_msg0xB2x1buf1 = PT_CAN_msg0xB2.buf[1];
          PT_CAN_msg0xB2x1buf2 = PT_CAN_msg0xB2.buf[2];
          PT_CAN_msg0xB2x1buf3 = PT_CAN_msg0xB2.buf[3];
          PT_CAN_msg0xB2x1buf4 = PT_CAN_msg0xB2.buf[4];
          PT_CAN_msg0xB2x1buf5 = PT_CAN_msg0xB2.buf[5];
          PT_CAN_msg0xB2x1buf6 = PT_CAN_msg0xB2.buf[6];
          PT_CAN_msg0xB2x1buf7 = PT_CAN_msg0xB2.buf[7];  


  //   word WhlSpdFL_Raw_CAN = (PT_CAN_msg0xB2.buf[0] + (PT_CAN_msg0xB2.buf[1] * 256));
  //   WhlSpdFL_Raw = WhlSpdFL_Raw_CAN;
  //   WhlSpdFL = (WhlSpdFL * 0.0075);

  //   word WhlSpdFR_Raw_CAN = (PT_CAN_msg0xB2.buf[0] + (PT_CAN_msg0xB2.buf[1] * 256));
  //   WhlSpdFR_Raw = WhlSpdFR_Raw_CAN;
  //   WhlSpdFR = (WhlSpdFR * 0.0075);

  //   word WhlSpdRL_Raw_CAN = (PT_CAN_msg0xB2.buf[0] + (PT_CAN_msg0xB2.buf[1] * 256));
  //   WhlSpdRL_Raw = WhlSpdRL_Raw_CAN;
  //   WhlSpdRL = (WhlSpdRL * 0.0075);

  //   word WhlSpdRR_Raw_CAN = (PT_CAN_msg0xB2.buf[0] + (PT_CAN_msg0xB2.buf[1] * 256));
  //   WhlSpdRR_Raw = WhlSpdRR_Raw_CAN;
  //   WhlSpdRR = (WhlSpdRR * 0.0075);


  // //  Serial.print("WhlSpdFL: "); Serial.print(WhlSpdFL); Serial.println();
  // //  Serial.print("WhlSpdFR: "); Serial.print(WhlSpdFR); Serial.println();
  // //  Serial.print("WhlSpdRL: "); Serial.print(WhlSpdRL); Serial.println();
  // //  Serial.print("WhlSpdRR: "); Serial.print(WhlSpdRR); Serial.println();

  // }





  }

  void canSniff0x86(const CAN_message_t &PT_CAN_msg0x86) { // function to sniff for OEM MQB SteeringWheelAngle


    // if (PT_CAN_msg0x86.buf[0] == 0x1D || PT_CAN_msg0x86.buf[0] == 0x1E || PT_CAN_msg0x86.buf[0] == 0x1F ) {

    //  Serial.print("MB "); Serial.print(PT_CAN_msg0x86.mb);
    //  Serial.print(" ID: "); Serial.print(PT_CAN_msg0x86.id, HEX);
    //  Serial.print(" Buffer: ");
    for (uint8_t i = 0; i < PT_CAN_msg0x86.len; i++)
    {
        //  Serial.print(PT_CAN_msg0x86.buf[i], HEX); Serial.print(" ");
    }
          // Serial.println("  ");

          PT_CAN_msg0x86_buf0_Raw = PT_CAN_msg0x86.buf[0];
          PT_CAN_msg0x86_buf1_Raw = PT_CAN_msg0x86.buf[1];
          PT_CAN_msg0x86_buf2_Raw = PT_CAN_msg0x86.buf[2];
          PT_CAN_msg0x86_buf3_Raw = PT_CAN_msg0x86.buf[3];
          PT_CAN_msg0x86_buf4_Raw = PT_CAN_msg0x86.buf[4];
          PT_CAN_msg0x86_buf5_Raw = PT_CAN_msg0x86.buf[5];
          PT_CAN_msg0x86_buf6_Raw = PT_CAN_msg0x86.buf[6];
          PT_CAN_msg0x86_buf7_Raw = PT_CAN_msg0x86.buf[7];  


  #pragma region // raw calcs from PT_CAN

    //   MQB_SteeringWheelAngle_0x86_CAN = (PT_CAN_msg0x86_buf2_Raw + (PT_CAN_msg0x86_buf3_Raw * 256) );
    //   MQB_SteeringWheelAngle_0x86_Raw = MQB_SteeringWheelAngle_0x86_CAN;


    //   if(MQB_SteeringWheelAngle_0x86_CAN < 100) { // steering wheel turned counter-clockwise
    //   MQB_SteeringWheelAngle_0x86_Float = map(MQB_SteeringWheelAngle_0x86_CAN,0,28,0,180);
    //   }

    // unsigned char myByteSWA_0x86x2 = (PT_CAN_msg0x86_buf2_Raw ); // Example byte
    // int startBit0x86x2 = 0; // Starting position of the range (0-indexed)
    // int numBits0x86x2 = 12;  // Number of bits to extract
    // // Create a mask with 'numBits' set to 1
    // unsigned char mask0x86x2 = (1 << numBits0x86x2) - 1; 
    // // Shift the desired range to the least significant position and then apply the mask
    // unsigned char extractedBits_SWA_0x86x2 = (myByteSWA_0x86x2 >> startBit0x86x2) & mask0x86x2;
    // unsigned char MQB_SteeringWheelAngle_0x86x2_Extracted = extractedBits_SWA_0x86x2;





    // unsigned char myByteSWA_0x86x3 = (PT_CAN_msg0x86_buf3_Raw ); // Example byte
    // int startBit0x86x3 = 0; // Starting position of the range (0-indexed)
    // int numBits0x86x3 = 4;  // Number of bits to extract
    // // Create a mask with 'numBits' set to 1
    // unsigned char mask0x86x3 = (1 << numBits0x86x3) - 1; 

    // // Shift the desired range to the least significant position and then apply the mask
    // unsigned char extractedBits_SWA_0x86x3 = (myByteSWA_0x86x3 >> startBit0x86x3) & mask0x86x3;

    // unsigned char MQB_SteeringWheelAngle_0x86x3_Extracted = extractedBits_SWA_0x86x3;


    // // MQB_SteeringWheelAngle_0x86_Raw = MQB_SteeringWheelAngle_0x86x3_Extracted;
    // MQB_SteeringWheelAngle_0x86_Raw = (MQB_SteeringWheelAngle_0x86x2_Extracted + (MQB_SteeringWheelAngle_0x86x3_Extracted * 256) );
    // MQB_SteeringWheelAngle_0x86_Int = MQB_SteeringWheelAngle_0x86_Raw;



        #pragma endregion


  }

  void canSniff0x104(const CAN_message_t &PT_CAN_msg0x104) { // function to sniff for Parking Brake Status

    //  Serial.print("MB "); Serial.print(PT_CAN_msg0x104.mb);
    //  Serial.print(" ID: "); Serial.print(PT_CAN_msg0x104.id, HEX);
    //  Serial.print(" Buffer: ");
    for (uint8_t i = 0; i < PT_CAN_msg0x104.len; i++)
    {
        //  Serial.print(PT_CAN_msg0x104.buf[i], HEX); Serial.print(" ");
    }
          // Serial.println("  ");


    word ParkBrakeStatus_0x104CAN = (PT_CAN_msg0x104.buf[7]);
    ParkBrakeStatus_0x104 = ParkBrakeStatus_0x104CAN;


  // Serial.print("ParkBrakeStatus_0x104: ");
  // Serial.print(ParkBrakeStatus_0x104);
  // Serial.println();

  }

  void canSniff0x106(const CAN_message_t &PT_CAN_msg0x106) { // function to sniff for Brake Pressures


    // if (PT_CAN_msg0x106.buf[0] == 0x1D || PT_CAN_msg0x106.buf[0] == 0x1E || PT_CAN_msg0x106.buf[0] == 0x1F ) {

    //  Serial.print("MB "); Serial.print(PT_CAN_msg0x106.mb);
    //  Serial.print(" ID: "); Serial.print(PT_CAN_msg0x106.id, HEX);
    //  Serial.print(" Buffer: ");
    for (uint8_t i = 0; i < PT_CAN_msg0x106.len; i++)
    {
        //  Serial.print(PT_CAN_msg0x106.buf[i], HEX); Serial.print(" ");
    }
          // Serial.println("  ");

          PT_CAN_msg0x106_buf0_Raw = PT_CAN_msg0x106.buf[0];
          PT_CAN_msg0x106_buf1_Raw = PT_CAN_msg0x106.buf[1];
          PT_CAN_msg0x106_buf2_Raw = PT_CAN_msg0x106.buf[2];
          PT_CAN_msg0x106_buf3_Raw = PT_CAN_msg0x106.buf[3];
          PT_CAN_msg0x106_buf4_Raw = PT_CAN_msg0x106.buf[4];
          PT_CAN_msg0x106_buf5_Raw = PT_CAN_msg0x106.buf[5];
          PT_CAN_msg0x106_buf6_Raw = PT_CAN_msg0x106.buf[6];
          PT_CAN_msg0x106_buf7_Raw = PT_CAN_msg0x106.buf[7];  

      // BrakePressureBAR_MasterCylfromAIM_0x106_CAN = (PT_CAN_msg0x106_buf2_Raw + (PT_CAN_msg0x106_buf3_Raw * 256));
      // BrakePressureBAR_MasterCylfromAIM_0x106_Raw = BrakePressureBAR_MasterCylfromAIM_0x106_CAN;
      // BrakePressureBAR_MasterCylfromAIM_Float = BrakePressureBAR_MasterCylfromAIM_0x106_Raw * 10.0;
      // BrakePressureBAR_MasterCylfromAIM_Int = BrakePressureBAR_MasterCylfromAIM_Float;

      // BrakePressureBAR_RearCylfromAIM_0x106_CAN = (PT_CAN_msg0x106_buf4_Raw + (PT_CAN_msg0x106_buf5_Raw * 256));
      // BrakePressureBAR_RearCylfromAIM_0x106_Raw = BrakePressureBAR_RearCylfromAIM_0x106_CAN;
      // BrakePressureBAR_RearCylfromAIM_Float = BrakePressureBAR_RearCylfromAIM_0x106_Raw * 10.0;
      // BrakePressureBAR_RearCylfromAIM_Int = BrakePressureBAR_RearCylfromAIM_Float;




  }

  void canSniff0x107(const CAN_message_t &PT_CAN_msg0x107) { // function to sniff for Tachometer EngineRPM

    //  Serial.print("MB "); Serial.print(PT_CAN_msg0x107.mb);
    //  Serial.print(" ID: "); Serial.print(PT_CAN_msg0x107.id, HEX);
    //  Serial.print(" Buffer: ");
    for (uint8_t i = 0; i < PT_CAN_msg0x107.len; i++)
    {
        //  Serial.print(PT_CAN_msg0x107.buf[i], HEX); Serial.print(" ");
    }
          // Serial.println("  ");
  }

  void canSniff0x120(const CAN_message_t &PT_CAN_msg0x120) { // function to determine STATIC position of the Cruise Control Stalk - OFF (forward/towards engine) or ON (backwards/towards driver)

      //  Serial.print("MB "); Serial.print(PT_CAN_msg0x120.mb);
      //  Serial.print(" ID: "); Serial.print(PT_CAN_msg0x120.id, HEX);
      //  Serial.print(" Buffer: ");
    for (uint8_t i = 0; i < PT_CAN_msg0x120.len; i++)
    {
            //  Serial.print(PT_CAN_msg0x120.buf[i], DEC); Serial.print(" ");
                  // digitalToggle(LED_PIN_CANRecieve);

    }
      //  Serial.println("  ");

    word cruiseStalkStaticPositionCAN0x120 = (PT_CAN_msg0x120.buf[3]);
    cruiseStalkStaticPosition_0x120 = cruiseStalkStaticPositionCAN0x120;

  }

  void canSniff0x12b(const CAN_message_t &PT_CAN_msg0x12B) { // function to determine STATUS of the Cruise Control Stalk Button

      //  Serial.print("MB "); Serial.print(PT_CAN_msg0x12B.mb);
      //  Serial.print(" ID: "); Serial.print(PT_CAN_msg0x12B.id, HEX);
      //  Serial.print(" Buffer: ");

    for (uint8_t i = 0; i < PT_CAN_msg0x12B.len; i++)
    {
            //  Serial.print(PT_CAN_msg0x12B.buf[i], DEC); Serial.print(" ");
                  // digitalToggle(LED_PIN_CANRecieve);

    }
      //  Serial.println("  ");

    cruiseStalk_byte1CAN = (PT_CAN_msg0x12B.buf[1]);
    cruiseStalk_byte1 = cruiseStalk_byte1CAN;

    cruiseStalkCAN = (PT_CAN_msg0x12B.buf[2]);
    cruiseStalk = cruiseStalkCAN;

  }

  void canSniff0x147(const CAN_message_t &PT_CAN_msg0x147) { // function to sniff status of Power/Torque Gauges on Instrument Cluster (odd 9bit values)

    //  Serial.print("MB "); Serial.print(PT_CAN_msg0x147.mb);
    //  Serial.print(" ID: "); Serial.print(PT_CAN_msg0x147.id, HEX);
    //  Serial.print(" Buffer: ");
    for (uint8_t i = 0; i < PT_CAN_msg0x147.len; i++)
    {
        //  Serial.print(PT_CAN_msg0x147.buf[i], HEX); Serial.print(" ");
    }
          // Serial.println("  ");

                 PT_CAN_msg0x147_buf0 = PT_CAN_msg0x147.buf[0];
                 PT_CAN_msg0x147_buf1 = PT_CAN_msg0x147.buf[1];
                 PT_CAN_msg0x147_buf2 = PT_CAN_msg0x147.buf[2];
                 PT_CAN_msg0x147_buf3 = PT_CAN_msg0x147.buf[3];
                 PT_CAN_msg0x147_buf4 = PT_CAN_msg0x147.buf[4];
                 PT_CAN_msg0x147_buf5 = PT_CAN_msg0x147.buf[5];
                 PT_CAN_msg0x147_buf6 = PT_CAN_msg0x147.buf[6];
                 PT_CAN_msg0x147_buf7 = PT_CAN_msg0x147.buf[7];



                // Serial.println();
                // Serial.print(PT_CAN_msg0x147.buf[0], HEX);     Serial.print(" ");
                // Serial.print(PT_CAN_msg0x147.buf[1], HEX);     Serial.print(" ");
                // Serial.print(PT_CAN_msg0x147.buf[2], HEX);     Serial.print(" ");
                // Serial.print(PT_CAN_msg0x147.buf[3], HEX);     Serial.print(" ");
                // Serial.print(PT_CAN_msg0x147.buf[4], HEX);     Serial.print(" ");
                // Serial.print(PT_CAN_msg0x147.buf[5], HEX);     Serial.print(" ");
                // Serial.print(PT_CAN_msg0x147.buf[6], HEX);     Serial.print(" ");
                // Serial.print(PT_CAN_msg0x147.buf[7], HEX);     Serial.print(" ");
                // Serial.println();



    word PowerGauge_9bit = (PT_CAN_msg0x147.buf[5]) +  (PT_CAN_msg0x147.buf[6]);
    word TorqueGauge_9bit =   (PT_CAN_msg0x147.buf[6]) +  (PT_CAN_msg0x147.buf[7]);

  
    PowerGauge_Converted = PowerGauge_9bit;
    TorqueGauge_Converted = TorqueGauge_9bit;

  // Serial.print("PaddlePositions: ");
  // Serial.print(PaddlePositions);
  // Serial.println();

  }

  void canSniff0x1AB(const CAN_message_t &PT_CAN_msg0x1AB) { // function to sniff for Brake Pedal Switch

    //  Serial.print("MB "); Serial.print(PT_CAN_msg0x1AB.mb);
    //  Serial.print(" ID: "); Serial.print(PT_CAN_msg0x1AB.id, HEX);
    //  Serial.print(" Buffer: ");
    for (uint8_t i = 0; i < PT_CAN_msg0x1AB.len; i++)
    {
        //  Serial.print(PT_CAN_msg0x1AB.buf[i], HEX); Serial.print(" ");
    }
          // Serial.println("  ");


    word BrakeSwitch_0x1AB_CAN = (PT_CAN_msg0x1AB.buf[6]);
    BrakeSwitch_0x1AB = BrakeSwitch_0x1AB_CAN;


  // Serial.print("BrakeSwitch_0x1AB: ");
  // Serial.print(BrakeSwitch_0x1AB);
  // Serial.println();

  }

  void canSniff0x366(const CAN_message_t &PT_CAN_msg0x366) { // function to sniff for hazard lights button

    //  Serial.print("MB "); Serial.print(PT_CAN_msg0x366.mb);
    //  Serial.print(" ID: "); Serial.print(PT_CAN_msg0x366.id, HEX);
    //  Serial.print(" Buffer: ");
    for (uint8_t i = 0; i < PT_CAN_msg0x366.len; i++)
    {
        //  Serial.print(PT_CAN_msg0x366.buf[i], HEX); Serial.print(" ");
    }
          // Serial.println("  ");


    word HazardLightsButton_0x366CAN = (PT_CAN_msg0x366.buf[2]);
    HazardLightsButton_0x366 = HazardLightsButton_0x366CAN;


  // Serial.print("HazardLightsButton_0x366: ");
  // Serial.print(HazardLightsButton_0x366);
  // Serial.println();

  }

  void canSniff0x385(const CAN_message_t &PT_CAN_msg0x385) { // function to sniff for Audi 8V Drive Select

    //  Serial.print("MB "); Serial.print(PT_CAN_msg0x385.mb);
    //  Serial.print(" ID: "); Serial.print(PT_CAN_msg0x385.id, HEX);
    //  Serial.print(" Buffer: ");
    for (uint8_t i = 0; i < PT_CAN_msg0x385.len; i++)
    {
        //  Serial.print(PT_CAN_msg0x385.buf[i], HEX); Serial.print(" ");
    }
          // Serial.println("  ");

    word DriveSelect_EngTrans_Mode_Raw_CAN = (PT_CAN_msg0x385.buf[2]);
    DriveSelect_EngTrans_Mode_Raw = DriveSelect_EngTrans_Mode_Raw_CAN;
    DriveSelect_EngTrans_Mode_Int = DriveSelect_EngTrans_Mode_Raw;

    word DriveSelect_Suspension_Mode_Raw_CAN = (PT_CAN_msg0x385.buf[4]);
    DriveSelect_Suspension_Mode_Raw = DriveSelect_Suspension_Mode_Raw_CAN;
    DriveSelect_Suspension_Mode_Int = DriveSelect_Suspension_Mode_Raw;

    word DriveSelect_Quattro_Mode_Raw_CAN = (PT_CAN_msg0x385.buf[3]);
    DriveSelect_Quattro_Mode_Raw = DriveSelect_Quattro_Mode_Raw_CAN;
    DriveSelect_Quattro_Mode_Int = DriveSelect_Quattro_Mode_Raw;

    word DriveSelect_Steering_Mode_Raw_CAN = (PT_CAN_msg0x385.buf[6]);
    DriveSelect_Steering_Mode_Raw = DriveSelect_Steering_Mode_Raw_CAN;
    DriveSelect_Steering_Mode_Int = DriveSelect_Steering_Mode_Raw;

    word DriveSelect_ACC_Mode_Raw_CAN = (PT_CAN_msg0x385.buf[6]);
    DriveSelect_ACC_Mode_Raw = DriveSelect_ACC_Mode_Raw_CAN;
    DriveSelect_ACC_Mode_Int = DriveSelect_ACC_Mode_Raw;




  // Serial.print("DriveSelect_EngTrans_Mode_Int: ");  Serial.print(DriveSelect_EngTrans_Mode_Int);  Serial.println();
  // Serial.print("DriveSelect_Suspension_Mode_Int: ");  Serial.print(DriveSelect_Suspension_Mode_Int);  Serial.println();
  // Serial.print("DriveSelect_Quattro_Mode_Int: ");  Serial.print(DriveSelect_Quattro_Mode_Int);  Serial.println();
  // Serial.print("DriveSelect_Steering_Mode_Int: ");  Serial.print(DriveSelect_Steering_Mode_Int);  Serial.println();
  // Serial.print("DriveSelect_ACC_Mode_Int: ");  Serial.print(DriveSelect_ACC_Mode_Int);  Serial.println();
  
  }

  void canSniff0x391(const CAN_message_t &PT_CAN_msg0x391) { // function to sniff for OBD/SAE Engine basics (pedal, load, cool temp, etc)

    //  Serial.print("MB "); Serial.print(PT_CAN_msg0x391.mb);
    //  Serial.print(" ID: "); Serial.print(PT_CAN_msg0x391.id, HEX);
    //  Serial.print(" Buffer: ");
    for (uint8_t i = 0; i < PT_CAN_msg0x391.len; i++)
    {
        //  Serial.print(PT_CAN_msg0x391.buf[i], HEX); Serial.print(" ");
    }
          // Serial.println("  ");

          PT_CAN_msg0x391_buf0_Raw = PT_CAN_msg0x391.buf[0];
          PT_CAN_msg0x391_buf1_Raw = PT_CAN_msg0x391.buf[1];
          PT_CAN_msg0x391_buf2_Raw = PT_CAN_msg0x391.buf[2];
          PT_CAN_msg0x391_buf3_Raw = PT_CAN_msg0x391.buf[3];
          PT_CAN_msg0x391_buf4_Raw = PT_CAN_msg0x391.buf[4];
          PT_CAN_msg0x391_buf5_Raw = PT_CAN_msg0x391.buf[5];
          PT_CAN_msg0x391_buf6_Raw = PT_CAN_msg0x391.buf[6];
          PT_CAN_msg0x391_buf7_Raw = PT_CAN_msg0x391.buf[7];  



    ThrottleTPS_0x391_CAN = (PT_CAN_msg0x391.buf[2]) ;
    ThrottleTPS_0x391 = (ThrottleTPS_0x391_CAN * 0.39215686275 ) ;


    AccelPedal_0x391_CAN = (PT_CAN_msg0x391.buf[5]) ;
    AccelPedal_0x391 = (AccelPedal_0x391_CAN * 0.39215686275 ) ;

    AccelPedal_0x391_Mapped_Float = map(AccelPedal_0x391,15,80,0,100);
    AccelPedal_0x391_Mapped = AccelPedal_0x391_Mapped_Float;
    AccelPedal_0x391_Mapped = constrain(AccelPedal_0x391_Mapped,0,100);

  // Serial.print("ThrottleTPS_0x391: ");
  // Serial.print(ThrottleTPS_0x391);
  // Serial.println();
  }

  void canSniff0x394(const CAN_message_t &PT_CAN_msg0x394) { // function to sniff for DQ500 Current Gear

    //  Serial.print("MB "); Serial.print(PT_CAN_msg0x394.mb);
    //  Serial.print(" ID: "); Serial.print(PT_CAN_msg0x394.id, HEX);
    //  Serial.print(" Buffer: ");
    for (uint8_t i = 0; i < PT_CAN_msg0x394.len; i++)
    {
        //  Serial.print(PT_CAN_msg0x394.buf[i], HEX); Serial.print(" ");
    }
          // Serial.println("  ");



    PT_CAN_msg0x394_buf0_Raw = PT_CAN_msg0x394.buf[0];
    PT_CAN_msg0x394_buf1_Raw = PT_CAN_msg0x394.buf[1];
    PT_CAN_msg0x394_buf2_Raw = PT_CAN_msg0x394.buf[2];
    PT_CAN_msg0x394_buf3_Raw = PT_CAN_msg0x394.buf[3];
    PT_CAN_msg0x394_buf4_Raw = PT_CAN_msg0x394.buf[4];
    PT_CAN_msg0x394_buf5_Raw = PT_CAN_msg0x394.buf[5];
    PT_CAN_msg0x394_buf6_Raw = PT_CAN_msg0x394.buf[6];
    PT_CAN_msg0x394_buf7_Raw = PT_CAN_msg0x394.buf[7];

    // DQ500_CurrentGear_0x394_CAN = (PT_CAN_msg0x394.buf[3]);
    // DQ500_CurrentGear_0x394 = DQ500_CurrentGear_0x394_CAN;



  // Serial.print("DQ500_CurrentGear_0x394: ");
  // Serial.print(DQ500_CurrentGear_0x394);
  // Serial.println();

  }

  void canSniff0x3DD(const CAN_message_t &PT_CAN_msg0x3DD) { // function to sniff status of Paddle Positions

    //  Serial.print("MB "); Serial.print(PT_CAN_msg0x3DD.mb);
    //  Serial.print(" ID: "); Serial.print(PT_CAN_msg0x3DD.id, HEX);
    //  Serial.print(" Buffer: ");
    for (uint8_t i = 0; i < PT_CAN_msg0x3DD.len; i++)
    {
        //  Serial.print(PT_CAN_msg0x3DD.buf[i], HEX); Serial.print(" ");
    }
          // Serial.println("  ");

                 PT_CAN_msg0x3DD_buf0 = PT_CAN_msg0x3DD.buf[0];
                 PT_CAN_msg0x3DD_buf1 = PT_CAN_msg0x3DD.buf[1];
                 PT_CAN_msg0x3DD_buf2 = PT_CAN_msg0x3DD.buf[2];
                 PT_CAN_msg0x3DD_buf3 = PT_CAN_msg0x3DD.buf[3];
                 PT_CAN_msg0x3DD_buf4 = PT_CAN_msg0x3DD.buf[4];
                 PT_CAN_msg0x3DD_buf5 = PT_CAN_msg0x3DD.buf[5];
                 PT_CAN_msg0x3DD_buf6 = PT_CAN_msg0x3DD.buf[6];
                 PT_CAN_msg0x3DD_buf7 = PT_CAN_msg0x3DD.buf[7];


                // Serial.print(PT_CAN_msg0x3DD.buf[0], HEX);     Serial.print(" ");
                // Serial.print(PT_CAN_msg0x3DD.buf[1], HEX);     Serial.print(" ");
                // Serial.print(PT_CAN_msg0x3DD.buf[2], HEX);     Serial.print(" ");
                // Serial.print(PT_CAN_msg0x3DD.buf[3], HEX);     Serial.print(" ");
                // Serial.print(PT_CAN_msg0x3DD.buf[4], HEX);     Serial.print(" ");
                // Serial.print(PT_CAN_msg0x3DD.buf[5], HEX);     Serial.print(" ");
                // Serial.print(PT_CAN_msg0x3DD.buf[6], HEX);     Serial.print(" ");
                // Serial.print(PT_CAN_msg0x3DD.buf[7], HEX);     Serial.print(" ");



    word PaddlePositionsCAN = (PT_CAN_msg0x3DD.buf[7]);
    PaddlePositions = PaddlePositionsCAN;

  // Serial.print("PaddlePositions: ");
  // Serial.print(PaddlePositions);
  // Serial.println();

  }

  void canSniff0x743(const CAN_message_t &PT_CAN_msg0x743) // function to sniff for DS1 data sent via 0x743
  { // function to determine the map switch status and activate BPV mosfet

    //  Serial.print("MB "); Serial.print(PT_CAN_msg0x743.mb);
    //  Serial.print(" ID: "); Serial.print(PT_CAN_msg0x743.id, HEX);
    //  Serial.print(" Buffer: ");
    for (uint8_t i = 0; i < PT_CAN_msg0x743.len; i++)
    {
        //  Serial.print(PT_CAN_msg0x743.buf[i], HEX); Serial.print(" ");
        // digitalToggle(LED_PIN_CANRecieve);
    }
          // Serial.println("  ");


      PT_CAN_msg0x743_buf0_Raw = (PT_CAN_msg0x743.buf[0]);
      PT_CAN_msg0x743_buf1_Raw = (PT_CAN_msg0x743.buf[1]);
      PT_CAN_msg0x743_buf2_Raw = (PT_CAN_msg0x743.buf[2]);
      PT_CAN_msg0x743_buf3_Raw = (PT_CAN_msg0x743.buf[3]);
      PT_CAN_msg0x743_buf4_Raw = (PT_CAN_msg0x743.buf[4]);
      PT_CAN_msg0x743_buf5_Raw = (PT_CAN_msg0x743.buf[5]);
      PT_CAN_msg0x743_buf6_Raw = (PT_CAN_msg0x743.buf[6]);
      PT_CAN_msg0x743_buf7_Raw = (PT_CAN_msg0x743.buf[7]);


  }
  
  void canSniff0x783(const CAN_message_t &PT_CAN_msg0x783) // function to sniff for DS1 data sent via 0x783
  { // function to determine the map switch status and activate BPV mosfet

    //  Serial.print("MB "); Serial.print(PT_CAN_msg0x783.mb);
    //  Serial.print(" ID: "); Serial.print(PT_CAN_msg0x783.id, HEX);
    //  Serial.print(" Buffer: ");
    for (uint8_t i = 0; i < PT_CAN_msg0x783.len; i++)
    {
        //  Serial.print(PT_CAN_msg0x783.buf[i], HEX); Serial.print(" ");
        // digitalToggle(LED_PIN_CANRecieve);
    }
          // Serial.println("  ");


    PT_CAN_msg0x783_buf0_Raw = (PT_CAN_msg0x783.buf[0]);
    PT_CAN_msg0x783_buf1_Raw = (PT_CAN_msg0x783.buf[1]);
    PT_CAN_msg0x783_buf2_Raw = (PT_CAN_msg0x783.buf[2]);
    PT_CAN_msg0x783_buf3_Raw = (PT_CAN_msg0x783.buf[3]);
    PT_CAN_msg0x783_buf4_Raw = (PT_CAN_msg0x783.buf[4]);
    PT_CAN_msg0x783_buf5_Raw = (PT_CAN_msg0x783.buf[5]);
    PT_CAN_msg0x783_buf6_Raw = (PT_CAN_msg0x783.buf[6]);
    PT_CAN_msg0x783_buf7_Raw = (PT_CAN_msg0x783.buf[7]);


  }

  void canSniff0x7EE(const CAN_message_t &PT_CAN_msg0x7EE) { // function to sniff DS1 Current MapSlot when RealDash is active (custom CAN Frames created from DS1)


    if (PT_CAN_msg0x7EE.buf[0] == 0x1D || PT_CAN_msg0x7EE.buf[0] == 0x1E || PT_CAN_msg0x7EE.buf[0] == 0x1F ) {

    //  Serial.print("MB "); Serial.print(PT_CAN_msg0x7EE.mb);
    //  Serial.print(" ID: "); Serial.print(PT_CAN_msg0x7EE.id, HEX);
    //  Serial.print(" Buffer: ");
    for (uint8_t i = 0; i < PT_CAN_msg0x7EE.len; i++)
    {
        //  Serial.print(PT_CAN_msg0x7EE.buf[i], HEX); Serial.print(" ");
    }
          // Serial.println("  ");


    word DQ500_CurrentGear_0x7EE_CAN = (PT_CAN_msg0x7EE.buf[5]);
    DQ500_CurrentGear_0x7EE = DQ500_CurrentGear_0x7EE_CAN;

  // Serial.print("DQ500_CurrentGear_0x7EE: ");
  // Serial.print(DQ500_CurrentGear_0x7EE);

  // Serial.println();

  }


      if (PT_CAN_msg0x7EE.buf[0] > 239 && PT_CAN_msg0x7EE.buf[0] < 250) {

    //  Serial.print("MB "); Serial.print(PT_CAN_msg0x7EE.mb);
    //  Serial.print(" ID: "); Serial.print(PT_CAN_msg0x7EE.id, HEX);
    //  Serial.print(" Buffer: ");
    for (uint8_t i = 0; i < PT_CAN_msg0x7EE.len; i++)
    {
        //  Serial.print(PT_CAN_msg0x7EE.buf[i], HEX); Serial.print(" ");
    }
          // Serial.println("  ");

    word DS1CurrentMapSlot_0x7EE_CAN = (PT_CAN_msg0x7EE.buf[0]);
    DS1CurrentMapSlot_0x7EE = (DS1CurrentMapSlot_0x7EE_CAN - 240);

    // Serial.print("DS1CurrentMapSlot_0x7EE: ");
    // Serial.print(DS1CurrentMapSlot_0x7EE);

    // Serial.println();

  }

  }

  #pragma region // 0x788-790

  void canSniff0x788(const CAN_message_t &PT_CAN_msg0x788) // function to sniff for Relayed (high speed) data from separate TFT Module
  { // function to determine the map switch status and activate BPV mosfet

    //  Serial.print("MB "); Serial.print(PT_CAN_msg0x788.mb);
    //  Serial.print(" ID: "); Serial.print(PT_CAN_msg0x788.id, HEX);
    //  Serial.print(" Buffer: ");
    for (uint8_t i = 0; i < PT_CAN_msg0x788.len; i++)
    {
        //  Serial.print(PT_CAN_msg0x788.buf[i], HEX); Serial.print(" ");
        // digitalToggle(LED_PIN_CANRecieve);
    }
          // Serial.println("  ");


    PT_CAN_msg0x788_buf0_Raw = (PT_CAN_msg0x788.buf[0]);
    PT_CAN_msg0x788_buf1_Raw = (PT_CAN_msg0x788.buf[1]);
    PT_CAN_msg0x788_buf2_Raw = (PT_CAN_msg0x788.buf[2]);
    PT_CAN_msg0x788_buf3_Raw = (PT_CAN_msg0x788.buf[3]);
    PT_CAN_msg0x788_buf4_Raw = (PT_CAN_msg0x788.buf[4]);
    PT_CAN_msg0x788_buf5_Raw = (PT_CAN_msg0x788.buf[5]);
    PT_CAN_msg0x788_buf6_Raw = (PT_CAN_msg0x788.buf[6]);
    PT_CAN_msg0x788_buf7_Raw = (PT_CAN_msg0x788.buf[7]);


  }


  void canSniff0x789(const CAN_message_t &PT_CAN_msg0x789) // function to sniff for Relayed (high speed) data from Syvecs AWD Controller
  { 

    //  Serial.print("MB "); Serial.print(PT_CAN_msg0x789.mb);
    //  Serial.print(" ID: "); Serial.print(PT_CAN_msg0x789.id, HEX);
    //  Serial.print(" Buffer: ");
    for (uint8_t i = 0; i < PT_CAN_msg0x789.len; i++)
    {
        //  Serial.print(PT_CAN_msg0x789.buf[i], HEX); Serial.print(" ");
        // digitalToggle(LED_PIN_CANRecieve);
    }
          // Serial.println("  ");


    PT_CAN_msg0x789_buf0_Raw = (PT_CAN_msg0x789.buf[0]);
    PT_CAN_msg0x789_buf1_Raw = (PT_CAN_msg0x789.buf[1]);
    PT_CAN_msg0x789_buf2_Raw = (PT_CAN_msg0x789.buf[2]);
    PT_CAN_msg0x789_buf3_Raw = (PT_CAN_msg0x789.buf[3]);
    PT_CAN_msg0x789_buf4_Raw = (PT_CAN_msg0x789.buf[4]);
    PT_CAN_msg0x789_buf5_Raw = (PT_CAN_msg0x789.buf[5]);
    PT_CAN_msg0x789_buf6_Raw = (PT_CAN_msg0x789.buf[6]);
    PT_CAN_msg0x789_buf7_Raw = (PT_CAN_msg0x789.buf[7]);


  }


  void canSniff0x790(const CAN_message_t &PT_CAN_msg0x790) // function to sniff for Relayed (high speed) data from Syvecs AWD Controller
  { // function to determine the map switch status and activate BPV mosfet

     Serial.print("MB "); Serial.print(PT_CAN_msg0x790.mb);
     Serial.print(" ID: "); Serial.print(PT_CAN_msg0x790.id, HEX);
     Serial.print(" Buffer: ");
    for (uint8_t i = 0; i < PT_CAN_msg0x790.len; i++)
    {
         Serial.print(PT_CAN_msg0x790.buf[i], HEX); Serial.print(" ");
        // digitalToggle(LED_PIN_CANRecieve);
    }
          Serial.println("  ");


    PT_CAN_msg0x790_buf0_Raw = (PT_CAN_msg0x790.buf[0]);
    PT_CAN_msg0x790_buf1_Raw = (PT_CAN_msg0x790.buf[1]);
    PT_CAN_msg0x790_buf2_Raw = (PT_CAN_msg0x790.buf[2]);
    PT_CAN_msg0x790_buf3_Raw = (PT_CAN_msg0x790.buf[3]);
    PT_CAN_msg0x790_buf4_Raw = (PT_CAN_msg0x790.buf[4]);
    PT_CAN_msg0x790_buf5_Raw = (PT_CAN_msg0x790.buf[5]);
    PT_CAN_msg0x790_buf6_Raw = (PT_CAN_msg0x790.buf[6]);
    PT_CAN_msg0x790_buf7_Raw = (PT_CAN_msg0x790.buf[7]);


  }

  #pragma endregion



#pragma endregion

#pragma region // canTx and Sniffing Service09 Functions - PT_CAN_ECU - 0x7xx

  void canSniffService09_PT_CAN_0x7E8(const CAN_message_t& PT_CAN_msg0x7E8_Service09) { // Global PT_CAN_CALlback to catch any CAN frame coming in

    //  Serial.println(" ");
    //  Serial.print("MB: ");
    //  Serial.print(PT_CAN_msg0x7E8_Service09.mb);
    //  Serial.print("  ID: 0x");
    //  Serial.print(PT_CAN_msg0x7E8_Service09.id, HEX);
    //  Serial.print("  EXT: ");
    //  Serial.print(PT_CAN_msg0x7E8_Service09.flags.extended);
    //  Serial.print("  LEN: ");
    //  Serial.print(PT_CAN_msg0x7E8_Service09.len);
    //  Serial.print(" DATA: ");
    for (uint8_t i = 0; i < 8; i++)
    {
      //  Serial.print(PT_CAN_msg0x7E8_Service09.buf[i], HEX);
      //  Serial.print(" ");
    }

    // Serial.println("");

  #pragma region // Recognize VIN messages, coordinate buffered messages

    if (PT_CAN_msg0x7E8_Service09.buf[0] == 0x10 && PT_CAN_msg0x7E8_Service09.buf[2] == 0x49 && PT_CAN_msg0x7E8_Service09.buf[3] == 0x2) {    ////////// Service$09 Positive Response message  and First Frame.
      PT_CAN_Service09_CF_Max = 2;
      PT_CAN_f_sid09VIN_multi = 1;   // SID$09 VIN process

      if (PT_CAN_msg0x7E8_Service09.buf[3] == 02) {   // Means Info Type == 02 VIN. InfoType == 04 PT_CAN_CALID.
        // ignore PT_CAN_msg0x7E8_Service09.buf[4]. because this buffer is always 0x01.Number of data item.
        PT_CAN_VIN_buf1 = PT_CAN_msg0x7E8_Service09.buf[5];
        PT_CAN_VIN_buf2 = PT_CAN_msg0x7E8_Service09.buf[6];
        PT_CAN_VIN_buf3 = PT_CAN_msg0x7E8_Service09.buf[7];  // VIN buffer is fixed 17 byte. 
        PT_CAN_STAT_FC_09 = 1;

        // Serial.println("");
        // Serial.println("");

        // Serial.print(PT_CAN_msg0x7E8_Service09.buf[0], HEX);     Serial.print(" ");
        // Serial.print(PT_CAN_msg0x7E8_Service09.buf[1], HEX);     Serial.print(" ");
        // Serial.print(PT_CAN_msg0x7E8_Service09.buf[2], HEX);     Serial.print(" ");
        // Serial.print(PT_CAN_msg0x7E8_Service09.buf[3], HEX);     Serial.print(" ");
        // Serial.print(PT_CAN_msg0x7E8_Service09.buf[4], HEX);     Serial.print(" ");
        // Serial.print(PT_CAN_msg0x7E8_Service09.buf[5], HEX);     Serial.print(" ");
        // Serial.print(PT_CAN_msg0x7E8_Service09.buf[6], HEX);     Serial.print(" ");
        // Serial.print(PT_CAN_msg0x7E8_Service09.buf[7], HEX);     Serial.print(" ");
        // Serial.println("");
        // Serial.println("");


      }

    }

    else if (PT_CAN_f_sid09VIN_multi == 1 && PT_CAN_msg0x7E8_Service09.buf[0] > 0x20 && PT_CAN_STAT_FC_09 == 0x2) {     // Means CF: Consecutive Frame and Buf[0] is sequence number
      PT_CAN_Service09_seq_Num = PT_CAN_msg0x7E8_Service09.buf[0]; //

      // Serial.print(PT_CAN_msg0x7E8_Service09.buf[0], HEX);     Serial.print(" ");
      // Serial.print(PT_CAN_msg0x7E8_Service09.buf[1], HEX);     Serial.print(" ");
      // Serial.print(PT_CAN_msg0x7E8_Service09.buf[2], HEX);     Serial.print(" ");
      // Serial.print(PT_CAN_msg0x7E8_Service09.buf[3], HEX);     Serial.print(" ");
      // Serial.print(PT_CAN_msg0x7E8_Service09.buf[4], HEX);     Serial.print(" ");
      // Serial.print(PT_CAN_msg0x7E8_Service09.buf[5], HEX);     Serial.print(" ");
      // Serial.print(PT_CAN_msg0x7E8_Service09.buf[6], HEX);     Serial.print(" ");
      // Serial.print(PT_CAN_msg0x7E8_Service09.buf[7], HEX);     Serial.print(" ");

      if (PT_CAN_Service09_seq_Num == 0x21) {   // Sequence Number is 1.
        PT_CAN_VIN_buf4 = PT_CAN_msg0x7E8_Service09.buf[1];
        PT_CAN_VIN_buf5 = PT_CAN_msg0x7E8_Service09.buf[2];
        PT_CAN_VIN_buf6 = PT_CAN_msg0x7E8_Service09.buf[3];
        PT_CAN_VIN_buf7 = PT_CAN_msg0x7E8_Service09.buf[4];
        PT_CAN_VIN_buf8 = PT_CAN_msg0x7E8_Service09.buf[5];
        PT_CAN_VIN_buf9 = PT_CAN_msg0x7E8_Service09.buf[6];
        PT_CAN_VIN_buf10 = PT_CAN_msg0x7E8_Service09.buf[7];
        PT_CAN_Service09_CF_Num = 1;
      }

      if (PT_CAN_Service09_seq_Num == 0x22) {  // Sequence Number is 2.
        PT_CAN_VIN_buf11 = PT_CAN_msg0x7E8_Service09.buf[1];
        PT_CAN_VIN_buf12 = PT_CAN_msg0x7E8_Service09.buf[2];
        PT_CAN_VIN_buf13 = PT_CAN_msg0x7E8_Service09.buf[3];
        PT_CAN_VIN_buf14 = PT_CAN_msg0x7E8_Service09.buf[4];
        PT_CAN_VIN_buf15 = PT_CAN_msg0x7E8_Service09.buf[5];
        PT_CAN_VIN_buf16 = PT_CAN_msg0x7E8_Service09.buf[6];
        PT_CAN_VIN_buf17 = PT_CAN_msg0x7E8_Service09.buf[7];
        PT_CAN_Service09_CF_Num = 2;
      }

      if (PT_CAN_Service09_CF_Num == PT_CAN_Service09_CF_Max) {

        if (PT_CAN_STAT_FC_09 == 2) {
          PT_CAN_STAT_FC_09 = 0;
          PT_CAN_Service09_CF_Num = 0;
          PT_CAN_f_sid09VIN_multi = 0;
        }

        //  Serial.println();

        // Serial.print("startVINbuffers: ");
        // Serial.print (PT_CAN_VIN_buf1);
        // Serial.print (PT_CAN_VIN_buf2);
        // Serial.print (PT_CAN_VIN_buf3);
        // Serial.print (PT_CAN_VIN_buf4);
        // Serial.print (PT_CAN_VIN_buf5);
        // Serial.print (PT_CAN_VIN_buf6);
        // Serial.print (PT_CAN_VIN_buf7);
        // Serial.print (PT_CAN_VIN_buf8);
        // Serial.print (PT_CAN_VIN_buf9);
        // Serial.print (PT_CAN_VIN_buf10);
        // Serial.print (PT_CAN_VIN_buf11);
        // Serial.print (PT_CAN_VIN_buf12);
        // Serial.print (PT_CAN_VIN_buf13);
        // Serial.print (PT_CAN_VIN_buf14);
        // Serial.print (PT_CAN_VIN_buf15);
        // Serial.print (PT_CAN_VIN_buf16);
        // Serial.print (PT_CAN_VIN_buf17);
        // Serial.print(",");
        // Serial.print("endbuffer"); Serial.print("\t");
        //  Serial.print(" PT_CAN_STAT_FC_09: "); Serial.print( PT_CAN_STAT_FC_09);


        PT_CAN_VIN_ReadComplete = 1;

        // Serial.println();
        // Serial.print("PT_CAN_VIN_ReadComplete:  ");         Serial.print(PT_CAN_VIN_ReadComplete); Serial.print("\t");
        // Serial.print("TFT_VehicleIdentificationComplete: "); Serial.print(TFT_VehicleIdentificationComplete);
        // Serial.println();


        // TFT_VehicleIdentificationComplete = 1;

      }
   
    }


  #pragma endregion

  #pragma region // Recognize ECU HW Name messages, coordinate buffered messages

    if (PT_CAN_msg0x7E8_Service09.buf[0] == 0x10 && PT_CAN_msg0x7E8_Service09.buf[2] == 0x49 && PT_CAN_msg0x7E8_Service09.buf[3] == 0xA){    ////////// Service$09 Positive Response message  and First Frame.
      PT_CAN_Service09_CF_Max = 3;
              PT_CAN_f_sid09ECUName_multi = 1;   // SID$09 PT_CAN_ECUName process
                
              if(PT_CAN_msg0x7E8_Service09.buf[3] == 0xA){   // Means Info Type == 02 = VIN, 04 = PT_CAN_CALID, 0A(dec 10) = PT_CAN_ECUName.
                  // ignore PT_CAN_msg0x7E8_Service09.buf[4]. because this buffer is always 0x01.Number of data item.
                  PT_CAN_ECUName_buf1 = PT_CAN_msg0x7E8_Service09.buf[5];
                  PT_CAN_ECUName_buf2 = PT_CAN_msg0x7E8_Service09.buf[6];
                  PT_CAN_ECUName_buf3 = PT_CAN_msg0x7E8_Service09.buf[7];  // PT_CAN_ECUName buffer is fixed 16 byte. 
                  PT_CAN_STAT_FC_09 = 1; 
                
                  // Serial.print(PT_CAN_msg0x7E8_Service09.buf[0], HEX);     Serial.print(" ");
                  // Serial.print(PT_CAN_msg0x7E8_Service09.buf[1], HEX);     Serial.print(" ");
                  // Serial.print(PT_CAN_msg0x7E8_Service09.buf[2], HEX);     Serial.print(" ");
                  // Serial.print(PT_CAN_msg0x7E8_Service09.buf[3], HEX);     Serial.print(" ");
                  // Serial.print(PT_CAN_msg0x7E8_Service09.buf[4], HEX);     Serial.print(" ");
                  // Serial.print(PT_CAN_msg0x7E8_Service09.buf[5], HEX);     Serial.print(" ");
                  // Serial.print(PT_CAN_msg0x7E8_Service09.buf[6], HEX);     Serial.print(" ");
                  // Serial.print(PT_CAN_msg0x7E8_Service09.buf[7], HEX);     Serial.print(" ");

              }
            }

      else if(PT_CAN_f_sid09ECUName_multi ==1 && PT_CAN_msg0x7E8_Service09.buf[0] >0x20 && PT_CAN_STAT_FC_09 ==2 ){     // Means CF: Consecutive Frame and Buf[0] is sequence number
        PT_CAN_Service09_seq_Num = PT_CAN_msg0x7E8_Service09.buf[0]; //
                  // Serial.print(PT_CAN_msg0x7E8_Service09.buf[0], HEX);     Serial.print(" ");
                  // Serial.print(PT_CAN_msg0x7E8_Service09.buf[1], HEX);     Serial.print(" ");
                  // Serial.print(PT_CAN_msg0x7E8_Service09.buf[2], HEX);     Serial.print(" ");
                  // Serial.print(PT_CAN_msg0x7E8_Service09.buf[3], HEX);     Serial.print(" ");
                  // Serial.print(PT_CAN_msg0x7E8_Service09.buf[4], HEX);     Serial.print(" ");
                  // Serial.print(PT_CAN_msg0x7E8_Service09.buf[5], HEX);     Serial.print(" ");
                  // Serial.print(PT_CAN_msg0x7E8_Service09.buf[6], HEX);     Serial.print(" ");
                  // Serial.print(PT_CAN_msg0x7E8_Service09.buf[7], HEX);     Serial.print(" ");

              if(PT_CAN_Service09_seq_Num == 0x21){   // Sequence Number is 1.
                  PT_CAN_ECUName_buf4  = PT_CAN_msg0x7E8_Service09.buf[1];
                  PT_CAN_ECUName_buf5  = PT_CAN_msg0x7E8_Service09.buf[2];
                  PT_CAN_ECUName_buf6  = PT_CAN_msg0x7E8_Service09.buf[3];
                  PT_CAN_ECUName_buf7  = PT_CAN_msg0x7E8_Service09.buf[4];
                  PT_CAN_ECUName_buf8  = PT_CAN_msg0x7E8_Service09.buf[5];
                  PT_CAN_ECUName_buf9  = PT_CAN_msg0x7E8_Service09.buf[6];
                  PT_CAN_ECUName_buf10 = PT_CAN_msg0x7E8_Service09.buf[7];
                  PT_CAN_Service09_CF_Num = 1;
                }

                if(PT_CAN_Service09_seq_Num == 0x22 ){  // Sequence Number is 2.
                  PT_CAN_ECUName_buf11 = PT_CAN_msg0x7E8_Service09.buf[1];
                  PT_CAN_ECUName_buf12 = PT_CAN_msg0x7E8_Service09.buf[2];
                  PT_CAN_ECUName_buf13 = PT_CAN_msg0x7E8_Service09.buf[3];
                  PT_CAN_ECUName_buf14 = PT_CAN_msg0x7E8_Service09.buf[4];
                  PT_CAN_ECUName_buf15 = PT_CAN_msg0x7E8_Service09.buf[5];
                  PT_CAN_ECUName_buf16 = PT_CAN_msg0x7E8_Service09.buf[6];
                  PT_CAN_ECUName_buf17 = PT_CAN_msg0x7E8_Service09.buf[7];
                  PT_CAN_Service09_CF_Num = 2;
                }

                if(PT_CAN_Service09_seq_Num == 0x23 ){  // Sequence Number is 3.
                  PT_CAN_ECUName_buf18 = PT_CAN_msg0x7E8_Service09.buf[1];
                  PT_CAN_ECUName_buf19 = PT_CAN_msg0x7E8_Service09.buf[2];
                  PT_CAN_ECUName_buf20 = PT_CAN_msg0x7E8_Service09.buf[3];
                  PT_CAN_ECUName_buf21 = PT_CAN_msg0x7E8_Service09.buf[4];
                  PT_CAN_ECUName_buf22 = PT_CAN_msg0x7E8_Service09.buf[5];
                  PT_CAN_ECUName_buf23 = PT_CAN_msg0x7E8_Service09.buf[6];
                  PT_CAN_ECUName_buf24 = PT_CAN_msg0x7E8_Service09.buf[7];
                  PT_CAN_Service09_CF_Num = 3;
                }

                if( PT_CAN_Service09_CF_Num == PT_CAN_Service09_CF_Max) {
                  if(PT_CAN_STAT_FC_09 == 2){
                    PT_CAN_STAT_FC_09=0;
                      PT_CAN_Service09_CF_Num = 0;
                      PT_CAN_f_sid09ECUName_multi = 0;               
                  }

            // Serial.println("");
            // Serial.print("startECU_HW_Namebuffers, printing consecutively: "); Serial.print("\t");
            // Serial.print(PT_CAN_ECUName_buf1); 
            // Serial.print(PT_CAN_ECUName_buf2); 
            // Serial.print(PT_CAN_ECUName_buf3); 
            // Serial.print(PT_CAN_ECUName_buf4); 
            // Serial.print(PT_CAN_ECUName_buf5); 
            // Serial.print(PT_CAN_ECUName_buf6); 
            // Serial.print(PT_CAN_ECUName_buf7); 
            // Serial.print(PT_CAN_ECUName_buf8); 
            // Serial.print(PT_CAN_ECUName_buf9); 
            // Serial.print(PT_CAN_ECUName_buf10); 
            // Serial.print(PT_CAN_ECUName_buf11); 
            // Serial.print(PT_CAN_ECUName_buf12); 
            // Serial.print(PT_CAN_ECUName_buf13); 
            // Serial.print(PT_CAN_ECUName_buf14); 
            // Serial.print(PT_CAN_ECUName_buf15); 
            // Serial.print(PT_CAN_ECUName_buf16); 
            // Serial.print(PT_CAN_ECUName_buf17); 
            // Serial.print(PT_CAN_ECUName_buf18); 
            // Serial.print(PT_CAN_ECUName_buf19); 
            // Serial.print(PT_CAN_ECUName_buf20); 
            // Serial.print(PT_CAN_ECUName_buf21); 
            // Serial.print(PT_CAN_ECUName_buf22); 
            // Serial.print(PT_CAN_ECUName_buf23); 
            // Serial.print(PT_CAN_ECUName_buf24); 
            // Serial.print(",");
            // Serial.print("endbuffer"); Serial.print("\t");


            PT_CAN_ECU_HW_ReadComplete = 1;
            // TFT_VehicleIdentificationComplete = 1;

            // Serial.println("");
            // Serial.print("PT_CAN_STAT_FC_09: "); Serial.print(PT_CAN_STAT_FC_09); Serial.print("\t");
            // Serial.print("TFT_VehicleIdentificationComplete: "); Serial.print(TFT_VehicleIdentificationComplete);
            // Serial.println("");
            // Serial.println();

                }
                
            //   Serial.print("PT_CAN_STAT_FC_09: "); Serial.print(PT_CAN_STAT_FC_09);
            //   Serial.println("");
            // Serial.print("PT_CAN_ECUNamebuf1: "); Serial.print(PT_CAN_ECUName_buf1); Serial.println("\t"); 
            // Serial.print("PT_CAN_ECUNamebuf2: "); Serial.print(PT_CAN_ECUName_buf2); Serial.println("\t"); 
            // Serial.print("PT_CAN_ECUNamebuf3: "); Serial.print(PT_CAN_ECUName_buf3); Serial.println("\t"); 
            // Serial.print("PT_CAN_ECUNamebuf4: "); Serial.print(PT_CAN_ECUName_buf4); Serial.println("\t"); 
            // Serial.print("PT_CAN_ECUNamebuf5: "); Serial.print(PT_CAN_ECUName_buf5); Serial.println("\t"); 
            // Serial.print("PT_CAN_ECUNamebuf6: "); Serial.print(PT_CAN_ECUName_buf6); Serial.println("\t"); 
            // Serial.print("PT_CAN_ECUNamebuf7: "); Serial.print(PT_CAN_ECUName_buf7); Serial.println("\t"); 
            // Serial.print("PT_CAN_ECUNamebuf8: "); Serial.print(PT_CAN_ECUName_buf8); Serial.println("\t"); 
            // Serial.print("PT_CAN_ECUNamebuf9: "); Serial.print(PT_CAN_ECUName_buf9); Serial.println("\t"); 
            // Serial.print("PT_CAN_ECUNamebuf10: "); Serial.print(PT_CAN_ECUName_buf10); Serial.println("\t"); 
            // Serial.print("PT_CAN_ECUNamebuf11: "); Serial.print(PT_CAN_ECUName_buf11); Serial.println("\t"); 
            // Serial.print("PT_CAN_ECUNamebuf12: "); Serial.print(PT_CAN_ECUName_buf12); Serial.println("\t"); 
            // Serial.print("PT_CAN_ECUNamebuf13: "); Serial.print(PT_CAN_ECUName_buf13); Serial.println("\t"); 

            // Serial.println("");
            // Serial.println("");
          
            // Serial.print("startECUNamebuffers, printing consecutively:  "); 
            // Serial.print(PT_CAN_ECUName_buf1); 
            // Serial.print(PT_CAN_ECUName_buf2); 
            // Serial.print(PT_CAN_ECUName_buf3); 
            // Serial.print(PT_CAN_ECUName_buf4); 
            // Serial.print(PT_CAN_ECUName_buf5); 
            // Serial.print(PT_CAN_ECUName_buf6); 
            // Serial.print(PT_CAN_ECUName_buf7); 
            // Serial.print(PT_CAN_ECUName_buf8); 
            // Serial.print(PT_CAN_ECUName_buf9); 
            // Serial.print(PT_CAN_ECUName_buf10); 
            // Serial.print(PT_CAN_ECUName_buf11); 
            // Serial.print(PT_CAN_ECUName_buf12); 
            // Serial.print(PT_CAN_ECUName_buf13); 
            // Serial.print(",");
            // Serial.print("endbuffer"); Serial.print("\t");

            // Serial.print("PT_CAN_STAT_FC_09: "); Serial.print(PT_CAN_STAT_FC_09);
            // Serial.println();
            // Serial.println("");

                    }

          // Serial.print("startECUNamebuffers, printing consecutively:  "); 
          // Serial.print(PT_CAN_ECUName_buf1); 
          // Serial.print(PT_CAN_ECUName_buf2); 
          // Serial.print(PT_CAN_ECUName_buf3); 
          // Serial.print(PT_CAN_ECUName_buf4); 
          // Serial.print(PT_CAN_ECUName_buf5); 
          // Serial.print(PT_CAN_ECUName_buf6); 
          // Serial.print(PT_CAN_ECUName_buf7); 
          // Serial.print(PT_CAN_ECUName_buf8); 
          // Serial.print(PT_CAN_ECUName_buf9); 
          // Serial.print(PT_CAN_ECUName_buf10); 
          // Serial.print(PT_CAN_ECUName_buf11); 
          // Serial.print(PT_CAN_ECUName_buf12); 
          // Serial.print(PT_CAN_ECUName_buf13); 
          // Serial.print(",");
          // Serial.print("endbuffer"); Serial.print("\t");
          
          // Serial.print("PT_CAN_STAT_FC_09: "); Serial.print(PT_CAN_STAT_FC_09);
          // Serial.println();
          // Serial.println("");
          
                          // }
          
          

  #pragma endregion

  #pragma region // Recognize PT_CAN_CALID messages, coordinate buffered messages

  if (PT_CAN_msg0x7E8_Service09.buf[0] == 0x10 && PT_CAN_msg0x7E8_Service09.buf[2] == 0x49 && PT_CAN_msg0x7E8_Service09.buf[3] == 0x4){    ////////// Service$09 Positive Response message  and First Frame.
            PT_CAN_Service09_CF_Max = 2;
            PT_CAN_f_sid09CALID_multi = 1;   // SID$09 PT_CAN_CALID process
              
            if(PT_CAN_msg0x7E8_Service09.buf[3] == 0x4){   // Means Info Type == 02 VIN. InfoType == 04 PT_CAN_CALID.
                // ignore PT_CAN_msg0x7E8_Service09.buf[4]. because this buffer is always 0x01.Number of data item.
                PT_CAN_CALID_buf1 = PT_CAN_msg0x7E8_Service09.buf[5];
                PT_CAN_CALID_buf2 = PT_CAN_msg0x7E8_Service09.buf[6];
                PT_CAN_CALID_buf3 = PT_CAN_msg0x7E8_Service09.buf[7];  // PT_CAN_CALID buffer is fixed 16 byte. 
                PT_CAN_STAT_FC_09 = 1; 
              
                // Serial.print(PT_CAN_msg0x7E8_Service09.buf[0], HEX);     Serial.print(" ");
                // Serial.print(PT_CAN_msg0x7E8_Service09.buf[1], HEX);     Serial.print(" ");
                // Serial.print(PT_CAN_msg0x7E8_Service09.buf[2], HEX);     Serial.print(" ");
                // Serial.print(PT_CAN_msg0x7E8_Service09.buf[3], HEX);     Serial.print(" ");
                // Serial.print(PT_CAN_msg0x7E8_Service09.buf[4], HEX);     Serial.print(" ");
                // Serial.print(PT_CAN_msg0x7E8_Service09.buf[5], HEX);     Serial.print(" ");
                // Serial.print(PT_CAN_msg0x7E8_Service09.buf[6], HEX);     Serial.print(" ");
                // Serial.print(PT_CAN_msg0x7E8_Service09.buf[7], HEX);     Serial.print(" ");

            }

          }

    else if(PT_CAN_f_sid09CALID_multi ==1 && PT_CAN_msg0x7E8_Service09.buf[0] >0x20 && PT_CAN_STAT_FC_09 == 2 ){   
        // Means CF: Consecutive Frame and Buf[0] is sequence number
        PT_CAN_Service09_seq_Num = PT_CAN_msg0x7E8_Service09.buf[0]; //
                // Serial.print(PT_CAN_msg0x7E8_Service09.buf[0], HEX);     Serial.print(" ");
                // Serial.print(PT_CAN_msg0x7E8_Service09.buf[1], HEX);     Serial.print(" ");
                // Serial.print(PT_CAN_msg0x7E8_Service09.buf[2], HEX);     Serial.print(" ");
                // Serial.print(PT_CAN_msg0x7E8_Service09.buf[3], HEX);     Serial.print(" ");
                // Serial.print(PT_CAN_msg0x7E8_Service09.buf[4], HEX);     Serial.print(" ");
                // Serial.print(PT_CAN_msg0x7E8_Service09.buf[5], HEX);     Serial.print(" ");
                // Serial.print(PT_CAN_msg0x7E8_Service09.buf[6], HEX);     Serial.print(" ");
                // Serial.print(PT_CAN_msg0x7E8_Service09.buf[7], HEX);     Serial.print(" ");

            if(PT_CAN_Service09_seq_Num == 0x21){   // Sequence Number is 1.
                PT_CAN_CALID_buf4  = PT_CAN_msg0x7E8_Service09.buf[1];
                PT_CAN_CALID_buf5  = PT_CAN_msg0x7E8_Service09.buf[2];
                PT_CAN_CALID_buf6  = PT_CAN_msg0x7E8_Service09.buf[3];
                PT_CAN_CALID_buf7  = PT_CAN_msg0x7E8_Service09.buf[4];
                PT_CAN_CALID_buf8  = PT_CAN_msg0x7E8_Service09.buf[5];
                PT_CAN_CALID_buf9  = PT_CAN_msg0x7E8_Service09.buf[6];
                PT_CAN_CALID_buf10 = PT_CAN_msg0x7E8_Service09.buf[7];
                PT_CAN_Service09_CF_Num = 1;
              }

              if(PT_CAN_Service09_seq_Num == 0x22 ){  // Sequence Number is 2.
                PT_CAN_CALID_buf11 = PT_CAN_msg0x7E8_Service09.buf[1];
                PT_CAN_CALID_buf12 = PT_CAN_msg0x7E8_Service09.buf[2];
                PT_CAN_CALID_buf13 = PT_CAN_msg0x7E8_Service09.buf[3];
                PT_CAN_CALID_buf14 = PT_CAN_msg0x7E8_Service09.buf[4];
                PT_CAN_CALID_buf15 = PT_CAN_msg0x7E8_Service09.buf[5];
                PT_CAN_CALID_buf16 = PT_CAN_msg0x7E8_Service09.buf[6];
                PT_CAN_CALID_buf17 = PT_CAN_msg0x7E8_Service09.buf[7];
                PT_CAN_Service09_CF_Num = 2;
              }

              if( PT_CAN_Service09_CF_Num == PT_CAN_Service09_CF_Max){

                if(PT_CAN_STAT_FC_09 == 2){
                  PT_CAN_STAT_FC_09=0;
                  PT_CAN_Service09_CF_Num = 0;
                    PT_CAN_f_sid09CALID_multi = 0;               
                }
                
                      // Serial.print("startCALIDbuffers: "); 
                      // Serial.print(PT_CAN_CALID_buf1); 
                      // Serial.print(PT_CAN_CALID_buf2); 
                      // Serial.print(PT_CAN_CALID_buf3); 
                      // Serial.print(PT_CAN_CALID_buf4); 
                      // Serial.print(PT_CAN_CALID_buf5); 
                      // Serial.print(PT_CAN_CALID_buf6); 
                      // Serial.print(PT_CAN_CALID_buf7); 
                      //  Serial.print(PT_CAN_CALID_buf8); 
                      //  Serial.print(PT_CAN_CALID_buf9); 
                      //  Serial.print(PT_CAN_CALID_buf10); 
                      //  Serial.print(PT_CAN_CALID_buf11); 
                      //  Serial.print(PT_CAN_CALID_buf12); 
                      //  Serial.print(PT_CAN_CALID_buf13); 
                      //  Serial.print(PT_CAN_CALID_buf14); 
                      //  Serial.print(PT_CAN_CALID_buf15); 
                      //  Serial.print(PT_CAN_CALID_buf16); 
                      //  Serial.print(PT_CAN_CALID_buf17); 
                      // Serial.print(",");
                      // Serial.print("endbuffer"); Serial.print("\t");


                      PT_CAN_ECU_CALID_ReadComplete = 1;
                      // TFT_VehicleIdentificationComplete = 1;

                      // Serial.println();
                      // Serial.print("PT_CAN_STAT_FC_09: "); Serial.print(PT_CAN_STAT_FC_09); Serial.print("\t");
                      // Serial.print("TFT_VehicleIdentificationComplete: "); Serial.print(TFT_VehicleIdentificationComplete);
                      // Serial.println();


              }


          //   Serial.print("PT_CAN_STAT_FC_09: "); Serial.print(PT_CAN_STAT_FC_09);

          // Serial.print("PT_CAN_CALID_buf1: "); Serial.print(PT_CAN_CALID_buf1); Serial.print("\t"); 
          // Serial.print("PT_CAN_CALID_buf2: "); Serial.print(PT_CAN_CALID_buf2); Serial.print("\t"); 
          // Serial.print("PT_CAN_CALID_buf3: "); Serial.print(PT_CAN_CALID_buf3); Serial.print("\t"); 
          // Serial.print("PT_CAN_CALID_buf4: "); Serial.print(PT_CAN_CALID_buf4); Serial.print("\t"); 
          // Serial.print("PT_CAN_CALID_buf5: "); Serial.print(PT_CAN_CALID_buf5); Serial.print("\t"); 
          // Serial.print("PT_CAN_CALID_buf6: "); Serial.print(PT_CAN_CALID_buf6); Serial.print("\t"); 
          // Serial.print("PT_CAN_CALID_buf7: "); Serial.print(PT_CAN_CALID_buf7); Serial.print("\t"); 
          // Serial.print("PT_CAN_CALID_buf8: "); Serial.print(PT_CAN_CALID_buf8); Serial.print("\t"); 
          // Serial.print("PT_CAN_CALID_buf9: "); Serial.print(PT_CAN_CALID_buf9); Serial.print("\t"); 
          // Serial.print("PT_CAN_CALID_buf10: "); Serial.print(PT_CAN_CALID_buf10); Serial.print("\t"); 
          // Serial.print("PT_CAN_CALID_buf11: "); Serial.print(PT_CAN_CALID_buf11); Serial.print("\t"); 
          // Serial.print("PT_CAN_CALID_buf12: "); Serial.print(PT_CAN_CALID_buf12); Serial.print("\t"); 
          // Serial.print("PT_CAN_CALID_buf13: "); Serial.print(PT_CAN_CALID_buf13); Serial.print("\t"); 

        //   Serial.print("startCALIDbuffers: "); Serial.print(PT_CAN_CALID_buf1); 
        //  Serial.print(PT_CAN_CALID_buf2); 
        //  Serial.print(PT_CAN_CALID_buf3); 
        //  Serial.print(PT_CAN_CALID_buf4); 
        //  Serial.print(PT_CAN_CALID_buf5); 
        //  Serial.print(PT_CAN_CALID_buf6); 
        //  Serial.print(PT_CAN_CALID_buf7); 
        //  Serial.print(PT_CAN_CALID_buf8); 
        //  Serial.print(PT_CAN_CALID_buf9); 
        //  Serial.print(PT_CAN_CALID_buf10); 
        //  Serial.print(PT_CAN_CALID_buf11); 
        //  Serial.print(PT_CAN_CALID_buf12); 
        //  Serial.print(PT_CAN_CALID_buf13); 
        //  Serial.print(",");
        //  Serial.print("endbuffer"); Serial.print("\t");
        //  Serial.print("PT_CAN_STAT_FC_09: "); Serial.print(PT_CAN_STAT_FC_09);
        //  Serial.println();

          // Serial.print("starting PT_CAN_CALID buffers, printing consecutively: "); Serial.print("\t");         
          // Serial.print(PT_CAN_CALID_buf2); 
          // Serial.print(PT_CAN_CALID_buf3); 
          // Serial.print(PT_CAN_CALID_buf4); 
          // Serial.print(PT_CAN_CALID_buf5); 
          // Serial.print(PT_CAN_CALID_buf6); 
          // Serial.print(PT_CAN_CALID_buf7); 
          // Serial.print(PT_CAN_CALID_buf8); 
          // Serial.print(PT_CAN_CALID_buf9); 
          // Serial.print(PT_CAN_CALID_buf10); 
          // Serial.print(PT_CAN_CALID_buf11); 
          // Serial.print(PT_CAN_CALID_buf12); 
          // Serial.print(PT_CAN_CALID_buf14); 
          // Serial.print(PT_CAN_CALID_buf15); 
          // Serial.print(PT_CAN_CALID_buf16); 
          // Serial.print(PT_CAN_CALID_buf17); 
          // Serial.print(",");
          // Serial.print("endbuffer"); Serial.print("\t");
          // Serial.print("PT_CAN_STAT_FC_09: "); Serial.print(PT_CAN_STAT_FC_09);
          // Serial.println();


                  }

#pragma endregion



  }


  void canTx_PT_CAN_09_Identification_VIN() { // function to request VIN via PT_CAN Service 09 Request
    
    CAN_message_t msgTx, msgRx;
    msgTx.buf[0] = 0x2;     // Data Length 2byte
    msgTx.buf[1] = 0x9;     // Service$01
    msgTx.buf[2] = 0x2;     // 
    msgTx.buf[3] = 0x55;        // Padding
    msgTx.buf[4] = 0x55;
    msgTx.buf[5] = 0x55;
    msgTx.buf[6] = 0x55;
    msgTx.buf[7] = 0x55;
    msgTx.len = 8;            // number of bytes in request
    msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
    //    msgTx.flags.extended = 1; // 29 bit header, not 11 bit 
    msgTx.flags.remote = 0;

    msgTx.id = 0x7DF; // request header for PT           // Physical Addressing Request to Engine ECU
    //  msgTx.id = 0x18DB33F1; // request header for PT Functional Addressing request to all emission related ECUs.
    TFTCAN3.write(msgTx);

    // Serial.println("");
    // Serial.print("canTx_PT_CAN_09_VIN request sent..."); 
    // Serial.println("");

  }

  void canTx_PT_CAN_09_Identification_ECU_CALID(){ // Request function to ask for PT_CAN_CALID via PT_CAN Service 09 Request
    
      CAN_message_t msgTx, msgRx;
      msgTx.buf[0] = 0x02;     // Data Length 2byte
      msgTx.buf[1] = 0x09;     // Service$01
      msgTx.buf[2] = 0x04;     // 
      msgTx.buf[3] = 0x55;        // Padding
      msgTx.buf[4] = 0x55;
      msgTx.buf[5] = 0x55; 
      msgTx.buf[6] = 0x55;
      msgTx.buf[7] = 0x55;
      msgTx.len = 8;            // number of bytes in request
      msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
  //    msgTx.flags.extended = 1; // 29 bit header, not 11 bit 
      msgTx.flags.remote = 0;
      
      msgTx.id = 0x7DF; // request header for PT           // Physical Addressing Request to Engine ECU
  //    msgTx.id = 0x18DB33F1; // request header for PT Functional Addressing request to all emission related ECUs.
      TFTCAN3.write(msgTx);

      // Serial.println("");
      // Serial.print("canTx_PT_CAN_09_Identification_ECU_CALID request sent..."); 
      // Serial.println("");

    }

  void canTx_PT_CAN_09_Identification_ECU_HW(){ // Request function to ask for ECU Name via PT_CAN Service 09 Request
    
      CAN_message_t msgTx, msgRx;
      msgTx.buf[0] = 0x02;     // Data Length 2byte
      msgTx.buf[1] = 0x09;     // Service$01
      msgTx.buf[2] = 0x0A;     // 
      msgTx.buf[3] = 0x55;        // Padding
      msgTx.buf[4] = 0x55;
      msgTx.buf[5] = 0x55; 
      msgTx.buf[6] = 0x55;
      msgTx.buf[7] = 0x55;
      msgTx.len = 8;            // number of bytes in request
      msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
  //    msgTx.flags.extended = 1; // 29 bit header, not 11 bit 
      msgTx.flags.remote = 0;
      
      msgTx.id = 0x7DF; // request header for PT           // Physical Addressing Request to Engine ECU
  //    msgTx.id = 0x18DB33F1; // request header for PT Functional Addressing request to all emission related ECUs.
      TFTCAN3.write(msgTx);

      // Serial.println("");
      // Serial.print("canTx_PT_CAN_09_Identification_ECU_HW request sent..."); 
      // Serial.println("");


    }

  void canTx_PT_CAN_09_Identification_ECU_Variable4(){ // Request function to ask for ECU Name via PT_CAN Service 09 Reques
    
      CAN_message_t msgTx, msgRx;
      msgTx.buf[0] = 0x02;     // Data Length 2byte
      msgTx.buf[1] = 0x09;     // Service$01
      msgTx.buf[2] = 0x0A;     // 
      msgTx.buf[3] = 0x55;        // Padding
      msgTx.buf[4] = 0x55;
      msgTx.buf[5] = 0x55; 
      msgTx.buf[6] = 0x55;
      msgTx.buf[7] = 0x55;
      msgTx.len = 8;            // number of bytes in request
      msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
  //    msgTx.flags.extended = 1; // 29 bit header, not 11 bit 
      msgTx.flags.remote = 0;
      
      msgTx.id = 0x7DF; // request header for PT           // Physical Addressing Request to Engine ECU
  //    msgTx.id = 0x18DB33F1; // request header for PT Functional Addressing request to all emission related ECUs.
      TFTCAN2.write(msgTx);

      // Serial.println("");
      // Serial.print("canTx_PT_CAN_09_Identification_ECU_Variable4 request sent..."); 
      // Serial.println("");


    }

  void canTx_PT_CAN_09_Identification_ECU_Variable5(){ // Request function to ask for ECU Name via PT_CAN Service 09 Reques
    
      CAN_message_t msgTx, msgRx;
      msgTx.buf[0] = 0x02;     // Data Length 2byte
      msgTx.buf[1] = 0x09;     // Service$01
      msgTx.buf[2] = 0x0A;     // 
      msgTx.buf[3] = 0x55;        // Padding
      msgTx.buf[4] = 0x55;
      msgTx.buf[5] = 0x55; 
      msgTx.buf[6] = 0x55;
      msgTx.buf[7] = 0x55;
      msgTx.len = 8;            // number of bytes in request
      msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
  //    msgTx.flags.extended = 1; // 29 bit header, not 11 bit 
      msgTx.flags.remote = 0;
      
      msgTx.id = 0x7DF; // request header for PT           // Physical Addressing Request to Engine ECU
  //    msgTx.id = 0x18DB33F1; // request header for PT Functional Addressing request to all emission related ECUs.
      TFTCAN2.write(msgTx);

      // Serial.println("");
      // Serial.print("canTx_PT_CAN_09_Identification_ECU_Variable5 request sent..."); 
      // Serial.println("");


    }

  void canTx_PT_CAN_09_Identification_ECU_Variable6(){ // Request function to ask for ECU Name via PT_CAN Service 09 Reques
    
      CAN_message_t msgTx, msgRx;
      msgTx.buf[0] = 0x02;     // Data Length 2byte
      msgTx.buf[1] = 0x09;     // Service$01
      msgTx.buf[2] = 0x0A;     // 
      msgTx.buf[3] = 0x55;        // Padding
      msgTx.buf[4] = 0x55;
      msgTx.buf[5] = 0x55; 
      msgTx.buf[6] = 0x55;
      msgTx.buf[7] = 0x55;
      msgTx.len = 8;            // number of bytes in request
      msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
  //    msgTx.flags.extended = 1; // 29 bit header, not 11 bit 
      msgTx.flags.remote = 0;
      
      msgTx.id = 0x7DF; // request header for PT           // Physical Addressing Request to Engine ECU
  //    msgTx.id = 0x18DB33F1; // request header for PT Functional Addressing request to all emission related ECUs.
      TFTCAN2.write(msgTx);

      // Serial.println("");
      // Serial.print("canTx_PT_CAN_09_Identification_ECU_Variable6 request sent..."); 
      // Serial.println("");


    }

  void canTx_PT_CAN_09_Identification_ECU_Variable7(){ // Request function to ask for ECU Name via PT_CAN Service 09 Request
    
      CAN_message_t msgTx, msgRx;
      msgTx.buf[0] = 0x02;     // Data Length 2byte
      msgTx.buf[1] = 0x09;     // Service$01
      msgTx.buf[2] = 0x0A;     // 
      msgTx.buf[3] = 0x55;        // Padding
      msgTx.buf[4] = 0x55;
      msgTx.buf[5] = 0x55; 
      msgTx.buf[6] = 0x55;
      msgTx.buf[7] = 0x55;
      msgTx.len = 8;            // number of bytes in request
      msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
  //    msgTx.flags.extended = 1; // 29 bit header, not 11 bit 
      msgTx.flags.remote = 0;
      
      msgTx.id = 0x7DF; // request header for PT           // Physical Addressing Request to Engine ECU
  //    msgTx.id = 0x18DB33F1; // request header for PT Functional Addressing request to all emission related ECUs.
      TFTCAN2.write(msgTx);

      // Serial.println("");
      // Serial.print("canTx_PT_CAN_09_Identification_ECU_Variable7 request sent..."); 
      // Serial.println("");


    }




  void canTx_PT_CAN_09_Identification_TCU_Variable1(){ // Request function to ask for TCU Variable1 via PT_CAN Service 09 Request
  
      CAN_message_t msgTx, msgRx;
      msgTx.buf[0] = 0x02;     // Data Length 2byte
      msgTx.buf[1] = 0x09;     // Service$01
      msgTx.buf[2] = 0x0A;     // 
      msgTx.buf[3] = 0x55;        // Padding
      msgTx.buf[4] = 0x55;
      msgTx.buf[5] = 0x55; 
      msgTx.buf[6] = 0x55;
      msgTx.buf[7] = 0x55;
      msgTx.len = 8;            // number of bytes in request
      msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
      //  msgTx.flags.extended = 1; // 29 bit header, not 11 bit 
      msgTx.flags.remote = 0;
      
      msgTx.id = 0x7DF; // request header for PT           // Physical Addressing Request to Engine ECU
      //  msgTx.id = 0x18DB33F1; // request header for PT Functional Addressing request to all emission related ECUs.
      TFTCAN2.write(msgTx);

      // Serial.println("");
      // Serial.print("canTx_PT_CAN_09_Identification_TCU_Variable1 request sent..."); 
      // Serial.println("");
    
      }
    
  void canTx_PT_CAN_09_Identification_TCU_Variable2(){ // Request function to ask for TCU Variable2 via PT_CAN Service 09 Request
    
      CAN_message_t msgTx, msgRx;
      msgTx.buf[0] = 0x02;     // Data Length 2byte
      msgTx.buf[1] = 0x09;     // Service$01
      msgTx.buf[2] = 0x0A;     // 
      msgTx.buf[3] = 0x55;        // Padding
      msgTx.buf[4] = 0x55;
      msgTx.buf[5] = 0x55; 
      msgTx.buf[6] = 0x55;
      msgTx.buf[7] = 0x55;
      msgTx.len = 8;            // number of bytes in request
      msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
      //  msgTx.flags.extended = 1; // 29 bit header, not 11 bit 
      msgTx.flags.remote = 0;
      
      msgTx.id = 0x7DF; // request header for PT           // Physical Addressing Request to Engine ECU
      //  msgTx.id = 0x18DB33F1; // request header for PT Functional Addressing request to all emission related ECUs.
      TFTCAN2.write(msgTx);

      // Serial.println("");
      // Serial.print("canTx_PT_CAN_09_Identification_TCU_Variable2 request sent..."); 
      // Serial.println("");
      
      }
    
  void canTx_PT_CAN_09_Identification_TCU_Variable3(){ // Request function to ask for TCU Variable2 via PT_CAN Service 09 Request
    
      CAN_message_t msgTx, msgRx;
      msgTx.buf[0] = 0x02;     // Data Length 2byte
      msgTx.buf[1] = 0x09;     // Service$01
      msgTx.buf[2] = 0x0A;     // 
      msgTx.buf[3] = 0x55;        // Padding
      msgTx.buf[4] = 0x55;
      msgTx.buf[5] = 0x55; 
      msgTx.buf[6] = 0x55;
      msgTx.buf[7] = 0x55;
      msgTx.len = 8;            // number of bytes in request
      msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
      //  msgTx.flags.extended = 1; // 29 bit header, not 11 bit 
      msgTx.flags.remote = 0;
      
      msgTx.id = 0x7DF; // request header for PT           // Physical Addressing Request to Engine ECU
      //  msgTx.id = 0x18DB33F1; // request header for PT Functional Addressing request to all emission related ECUs.
      TFTCAN2.write(msgTx);

      // Serial.println("");
      // Serial.print("canTx_PT_CAN_09_Identification_TCU_Variable3 request sent..."); 
      // Serial.println("");
        
        }
      
  void canTx_PT_CAN_09_Identification_TCU_Variable4(){ // Request function to ask for TCU Variable2 via PT_CAN Service 09 Request
    
      CAN_message_t msgTx, msgRx;
      msgTx.buf[0] = 0x02;     // Data Length 2byte
      msgTx.buf[1] = 0x09;     // Service$01
      msgTx.buf[2] = 0x0A;     // 
      msgTx.buf[3] = 0x55;        // Padding
      msgTx.buf[4] = 0x55;
      msgTx.buf[5] = 0x55; 
      msgTx.buf[6] = 0x55;
      msgTx.buf[7] = 0x55;
      msgTx.len = 8;            // number of bytes in request
      msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
      //  msgTx.flags.extended = 1; // 29 bit header, not 11 bit 
      msgTx.flags.remote = 0;
      
      msgTx.id = 0x7DF; // request header for PT           // Physical Addressing Request to Engine ECU
      //  msgTx.id = 0x18DB33F1; // request header for PT Functional Addressing request to all emission related ECUs.
      TFTCAN2.write(msgTx);

      // Serial.println("");
      // Serial.print("canTx_PT_CAN_09_Identification_TCU_Variable4 request sent..."); 
      // Serial.println("");
      
      }
        
      

  void canTx_PT_CAN_09_FlowControl() { // function to manage flowcontrol buffer via PT_CAN Service 09 Request
    CAN_message_t msgTx, msgRx;
    msgTx.buf[0] = 0x30;     // Data Length 2byte
    msgTx.buf[1] = 0x00;     // Service$01
    msgTx.buf[2] = 0x00;     // 
    msgTx.buf[3] = 0x00;        // Padding
    msgTx.buf[4] = 0x00;
    msgTx.buf[5] = 0x00;
    msgTx.buf[6] = 0x00;
    msgTx.buf[7] = 0x00;
    msgTx.len = 8;            // number of bytes in request
    // msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
    msgTx.flags.extended = 0; // 29 bit header, not 11 bit 
    msgTx.flags.remote = 0;


    msgTx.id = 0x7E0; // request header for PT_CAN           // Physical Addressing Request to Engine ECU
    // msgTx.id = 0x18DA11F1; // request header for PT_CAN Functional Addressing request to all emission related ECUs.
  //  msgTx.id = 0x18DB33F1; // request header for PT_CAN Functional Addressing request to all emission related ECUs.
    TFTCAN3.write(msgTx);
    PT_CAN_STAT_FC_09 = 2;  // Flow Control sent

    // Serial.println("");
    // Serial.print("canTX_PT_CAN_09_FlowControl message sent..."); 
    // Serial.println("");
  

  }





#pragma endregion

#pragma region // canTx and Sniffing Service22 Functions - PT_CAN_ECU - 0x7E8

    #pragma region //canSniff Function

      void canSniffService22_PT_CAN_ECU_0x7E8 (const CAN_message_t &PT_CAN_msg0x7E8){ // Sniffing of 0x7E8 (0x7E8 is a response to request sent to 0x7E0 or 0x700)

      // if((PT_CAN_msg0x7E8.buf[0] == 0x1) && (PT_CAN_msg0x7E8.buf[1] == 0x4)) {
        
        // Serial.println(" ");
        // Serial.print("MB: ");
        // Serial.print(PT_CAN_msg0x7E8.mb);
        // Serial.print("  ID: 0x");
        // Serial.print(PT_CAN_msg0x7E8.id, HEX);
        // Serial.print("  EXT: ");
        // Serial.print(PT_CAN_msg0x7E8.flags.extended);
        // Serial.print("  LEN: ");
        // Serial.print(PT_CAN_msg0x7E8.len);
        // Serial.print(" DATA: ");
          for (uint8_t i = 0; i < 8; i++)
            {
            // Serial.print(PT_CAN_msg0x7E8.buf[i], HEX);
            // Serial.print(" ");
            }

          // Serial.println("");

      //}





        #pragma region // Filter 0x7E8 Response Frames, coordinate buffered messages - First byte = 0x3

          if (PT_CAN_msg0x7E8.buf[0] == 0x03 && PT_CAN_msg0x7E8.buf[1] == 0x6e){    ////////// Positive Response message  and First Frame.
                    PT_CAN_ECU_0x7E8_CF_Max = 1;
                    PT_CAN_f_sid22_0x7E8_multi = 1;   // 
                      
                    if(PT_CAN_msg0x7E8.buf[2] == 0xF1){   // 

                        PT_CAN_msg0x7E8_buf0 = PT_CAN_msg0x7E8.buf[0];
                        PT_CAN_msg0x7E8_buf1 = PT_CAN_msg0x7E8.buf[1];
                        PT_CAN_msg0x7E8_buf2 = PT_CAN_msg0x7E8.buf[2];
                        PT_CAN_msg0x7E8_buf3 = PT_CAN_msg0x7E8.buf[3];
                        PT_CAN_msg0x7E8_buf4 = PT_CAN_msg0x7E8.buf[4];
                        PT_CAN_msg0x7E8_buf5 = PT_CAN_msg0x7E8.buf[5];
                        PT_CAN_msg0x7E8_buf6 = PT_CAN_msg0x7E8.buf[6];
                        PT_CAN_msg0x7E8_buf7 = PT_CAN_msg0x7E8.buf[7];  
                        PT_CAN_ECU_0x7E8_STAT_FC_22 = 1; 
                      
                        // Serial.print(PT_CAN_msg0x7E8.buf[0], HEX);     Serial.print(" ");
                        // Serial.print(PT_CAN_msg0x7E8.buf[1], HEX);     Serial.print(" ");
                        // Serial.print(PT_CAN_msg0x7E8.buf[2], HEX);     Serial.print(" ");
                        // Serial.print(PT_CAN_msg0x7E8.buf[3], HEX);     Serial.print(" ");
                        // Serial.print(PT_CAN_msg0x7E8.buf[4], HEX);     Serial.print(" ");
                        // Serial.print(PT_CAN_msg0x7E8.buf[5], HEX);     Serial.print(" ");
                        // Serial.print(PT_CAN_msg0x7E8.buf[6], HEX);     Serial.print(" ");
                        // Serial.print(PT_CAN_msg0x7E8.buf[7], HEX);     Serial.print(" ");

                    }
                  }    
            else if(PT_CAN_f_sid22_0x7E8_multi ==1 && PT_CAN_msg0x7E8.buf[0] >20 && PT_CAN_ECU_0x7E8_STAT_FC_22 ==2 ){     // Means CF: Consecutive Frame and Buf[0] is sequence number
                PT_CAN_ECU_0x7E8_seq_Num = PT_CAN_msg0x7E8.buf[0]; //
                        //  Serial.print("Starting ElseIF  ");

                        // Serial.print(PT_CAN_msg0x7E8.buf[0], HEX);     Serial.print(" ");
                        // Serial.print(PT_CAN_msg0x7E8.buf[1], HEX);     Serial.print(" ");
                        // Serial.print(PT_CAN_msg0x7E8.buf[2], HEX);     Serial.print(" ");
                        // Serial.print(PT_CAN_msg0x7E8.buf[3], HEX);     Serial.print(" ");
                        // Serial.print(PT_CAN_msg0x7E8.buf[4], HEX);     Serial.print(" ");
                        // Serial.print(PT_CAN_msg0x7E8.buf[5], HEX);     Serial.print(" ");
                        // Serial.print(PT_CAN_msg0x7E8.buf[6], HEX);     Serial.print(" ");
                        // Serial.print(PT_CAN_msg0x7E8.buf[7], HEX);     Serial.print(" ");

                    if(PT_CAN_ECU_0x7E8_seq_Num == 0x21){   // Sequence Number is 1.
                        PT_CAN_msg0x7E8_CF21_buf1  = PT_CAN_msg0x7E8.buf[1];
                        PT_CAN_msg0x7E8_CF21_buf2  = PT_CAN_msg0x7E8.buf[2];
                        PT_CAN_msg0x7E8_CF21_buf3  = PT_CAN_msg0x7E8.buf[3];
                        PT_CAN_msg0x7E8_CF21_buf4  = PT_CAN_msg0x7E8.buf[4];
                        PT_CAN_msg0x7E8_CF21_buf5  = PT_CAN_msg0x7E8.buf[5];
                        PT_CAN_msg0x7E8_CF21_buf6  = PT_CAN_msg0x7E8.buf[6];
                        PT_CAN_msg0x7E8_CF21_buf7 = PT_CAN_msg0x7E8.buf[7]; // last byte not used
                        // PT_CAN_CF_Num = 1;
                        PT_CAN_ECU_0x7E8_CF_Num = 1;
                        // Serial.print("CFNum = 1");

                      }
                      if( PT_CAN_ECU_0x7E8_CF_Num == PT_CAN_ECU_0x7E8_CF_Max){
                        if( PT_CAN_ECU_0x7E8_STAT_FC_22 == 2){
                            PT_CAN_ECU_0x7E8_STAT_FC_22 = 0;
                          // Serial.print("CFMax: ");

                            PT_CAN_ECU_0x7E8_CF_Num = 0;
                            PT_CAN_f_sid22_0x7E8_multi = 0;   
                                    //  Serial.print("CFMax: ");
                                    //  Serial.print(CFNum);
                    
                        }
                  

        // Serial.println();

                  }
                  }

                  
        #pragma endregion

        #pragma region // Recognize 0x7E8 Response Frames, coordinate buffered messages - LPFP Activations

          #pragma region // LPFP Activation Status

            if (PT_CAN_msg0x7E8.buf[0] == 0x3 && PT_CAN_msg0x7E8.buf[2] == 0x22 && PT_CAN_msg0x7E8.buf[3] == 0x31){    //
            ////// Second reply that the ECU has recieved the request to Activate/De-Activate LPFP,     

            PT_CAN_LPFP_Initiation_FirstReplyRecieved = 1;

            Serial.println("First Initiation Reply (3_7F_22_31) Reply Recieved...");

                    }

            if (PT_CAN_msg0x7E8.buf[0] == 0x3 && PT_CAN_msg0x7E8.buf[2] == 0x2E && PT_CAN_msg0x7E8.buf[3] == 0x31){    //
            

              PT_CAN_LPFP_Initiation_SecondReplyRecieved = 1;

            Serial.println("Second Initiation Reply (3_7F_2E_31) Reply Recieved...");

                    }

            if (PT_CAN_msg0x7E8.buf[0] == 0x30 && PT_CAN_msg0x7E8.buf[1] == 0x0 && PT_CAN_msg0x7E8.buf[2] == 0x0 && PT_CAN_msg0x7E8.buf[3] == 0x55 ){    //
            

              PT_CAN_LPFP_Initiation_ThirdReplyRecieved = 1;

              Serial.println("Third Initiation Reply (30_0_0_55) Reply Recieved...");

                    }

            if (PT_CAN_msg0x7E8.buf[0] == 0x3 && PT_CAN_msg0x7E8.buf[1] == 0x6E && PT_CAN_msg0x7E8.buf[2] == 0xF1 && PT_CAN_msg0x7E8.buf[3] == 0x98 ){    //
            

              PT_CAN_LPFP_Initiation_FourthReplyRecieved = 1;

              Serial.println("Fourth Initiation Reply (21_69_69_0) Reply Recieved...");

                    }
                    
            if (PT_CAN_msg0x7E8.buf[0] == 0x4 && PT_CAN_msg0x7E8.buf[3] == 0x3 && PT_CAN_msg0x7E8.buf[4] == 0xE5){    //
            ////// Second reply that the ECU has recieved the request to Activate/De-Activate LPFP, byte 2 is the status byte (1 = activate, 2 = de-activate)   
            
            
                PT_CAN_LPFP_Activation_Status_SecondReplyRecieved = 1;

                word PT_CAN_LPFP_Activation_Status_CAN = (PT_CAN_msg0x7E8.buf[2]);

                PT_CAN_LPFP_Activation_Status = PT_CAN_LPFP_Activation_Status_CAN;

                Serial.println("Message2 Reply Recieved...");

                if ( PT_CAN_LPFP_Activation_Status == 1 ) {
                    
                  PT_CAN_LPFP_Activation_Mode_Active = 1;

                }

                else {
                    PT_CAN_LPFP_Activation_Mode_Active = 0;
                    }

              }



          #pragma endregion

          #pragma region // LPFP Activation Continuous Messages

          if (PT_CAN_msg0x7E8.buf[0] == 0x4 && PT_CAN_msg0x7E8.buf[1] == 0x62 && PT_CAN_msg0x7E8.buf[2] == 0x1 && PT_CAN_msg0x7E8.buf[3] == 0x2 ){    //
          ////// Reply to first "Continue to activate LPFP" message sent to ECU
          // ** PT_CAN_msg0x7E8.buf[4] might = 0x0 or 0xC0 - yet to determine verify involved 

              PT_CAN_LPFP_Activation_Continuous_FirstReplyRecieved = 1;
              PT_CAN_LPFP_Activation_Continuous_FirstMessage_TimeSinceSignal = 0;

                  }

          if (PT_CAN_msg0x7E8.buf[0] == 0x5 && PT_CAN_msg0x7E8.buf[1] == 0x62 && PT_CAN_msg0x7E8.buf[2] == 0x1 && PT_CAN_msg0x7E8.buf[3] == 0x4 ){    //
          ////// Reply to second "Continue to activate LPFP" message sent to ECU

              PT_CAN_LPFP_Activation_Continuous_SecondReplyRecieved = 1;
              PT_CAN_LPFP_Activation_Continuous_SecondMessage_TimeSinceSignal = 0;
                  }




        #pragma endregion

          #pragma region // Fuel Tank Level (via Service22 Requests) 

          if (PT_CAN_msg0x7E8.buf[0] == 0x00 && PT_CAN_msg0x7E8.buf[1] == 0x00){    // Response to Service22 Requests for Fuel Tank Level
                
                PT_CAN_msg0x7E8_buf0 = PT_CAN_msg0x7E8.buf[0];
                PT_CAN_msg0x7E8_buf1 = PT_CAN_msg0x7E8.buf[1];
                PT_CAN_msg0x7E8_buf2 = PT_CAN_msg0x7E8.buf[2];
                PT_CAN_msg0x7E8_buf3 = PT_CAN_msg0x7E8.buf[3];
                PT_CAN_msg0x7E8_buf4 = PT_CAN_msg0x7E8.buf[4];
                PT_CAN_msg0x7E8_buf5 = PT_CAN_msg0x7E8.buf[5];
                PT_CAN_msg0x7E8_buf6 = PT_CAN_msg0x7E8.buf[6];
                PT_CAN_msg0x7E8_buf7 = PT_CAN_msg0x7E8.buf[7];  

                word FuelTankLevelCAN = PT_CAN_msg0x7E8_buf7;
                FuelTankLevel = FuelTankLevelCAN;
              
            }

          #pragma endregion

        #pragma endregion

      }

    #pragma endregion

    #pragma region // Functions - PT_CAN ECU DTC Clear

      void canTx_PT_CAN_22_0x700_ECU_DTC_Clear_4_14_FF(){ // function to send message to clear DTCs via PT_CAN 
        CAN_message_t msgTx, msgRx;
          msgTx.buf[0] = 0x4;     // Data Length 2byte
          msgTx.buf[1] = 0x14;     // 
          msgTx.buf[2] = 0xFF;     // 
          msgTx.buf[3] = 0xFF;        // Padding
          msgTx.buf[4] = 0xFF;
          msgTx.buf[5] = 0x00; 
          msgTx.buf[6] = 0x00;
          msgTx.buf[7] = 0x00;
          msgTx.len = 8;            // number of bytes in request
          msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
      //    msgTx.flags.extended = 1; // 29 bit header, not 11 bit 
          msgTx.flags.remote = 0;
          
          msgTx.id = 0x700; // request header for PT           // Physical Addressing Request to Engine ECU
        //  msgTx.id = 0x18DB33F1; // request header for PT Functional Addressing request to all emission related ECUs.
          TFTCAN3.write(msgTx);
          Serial.println();
          Serial.print("canTx_PT_CAN_22_0x700_ECU_DTC_Clear_4_14_FF Sent  ");  Serial.print("\t"); 
          Serial.println();
          
          // loopDelayPT_CAN_DTC_Clear = 0;

        }

      void canTx_PT_CAN_22_0x700_ECU_DTC_Clear_1_4_00(){ // function to send message to clear DTCs via PT_CAN 
        CAN_message_t msgTx, msgRx;
          msgTx.buf[0] = 0x1;     // Data Length 2byte
          msgTx.buf[1] = 0x4;     // 
          msgTx.buf[2] = 0x00;     // 
          msgTx.buf[3] = 0x00;        // Padding
          msgTx.buf[4] = 0x00;
          msgTx.buf[5] = 0x00; 
          msgTx.buf[6] = 0x00;
          msgTx.buf[7] = 0x00;
          msgTx.len = 8;            // number of bytes in request
          msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
      //    msgTx.flags.extended = 1; // 29 bit header, not 11 bit 
          msgTx.flags.remote = 0;
          
          msgTx.id = 0x700; // request header for PT           // Physical Addressing Request to Engine ECU
        //  msgTx.id = 0x18DB33F1; // request header for PT Functional Addressing request to all emission related ECUs.
          TFTCAN3.write(msgTx);
          Serial.println();
          Serial.print("canTx_PT_CAN_22_0x700_ECU_DTC_Clear_1_4_00 Sent  ");  Serial.print("\t"); 
          Serial.println();
          
          // loopDelayPT_CAN_DTC_Clear = 0;

        }

      void canTx_PT_CAN_22_0x700_ECU_DTC_Clear_1_4_55_55(){ // function to send message to clear DTCs via PT_CAN 
        CAN_message_t msgTx, msgRx;
          msgTx.buf[0] = 0x01;     // Data Length 2byte
          msgTx.buf[1] = 0x04;     // 
          msgTx.buf[2] = 0x55;     // 
          msgTx.buf[3] = 0x55;        // Padding
          msgTx.buf[4] = 0x55;
          msgTx.buf[5] = 0x55; 
          msgTx.buf[6] = 0x55;
          msgTx.buf[7] = 0x55;
          msgTx.len = 8;            // number of bytes in request
          msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
      //    msgTx.flags.extended = 1; // 29 bit header, not 11 bit 
          msgTx.flags.remote = 0;
          
          msgTx.id = 0x700; // request header for PT           // Physical Addressing Request to Engine ECU
        //  msgTx.id = 0x18DB33F1; // request header for PT Functional Addressing request to all emission related ECUs.
          TFTCAN3.write(msgTx);
          Serial.println();
          Serial.print("canTx_PT_CAN_22_0x700_ECU_DTC_Clear_1_4_55_55 Sent  ");  Serial.print("\t"); 
          Serial.println();
          
          // loopDelayPT_CAN_DTC_Clear = 0;

        }

      void canTx_PT_CAN_22_0x7E0_ECU_DTC_Clear_1_4_00(){ // function to send message to clear DTCs via PT_CAN - ECU only
        CAN_message_t msgTx, msgRx;
          msgTx.buf[0] = 0x01;     // Data Length 2byte
          msgTx.buf[1] = 0x04;     // 
          msgTx.buf[2] = 0x00;     // 
          msgTx.buf[3] = 0x00;        // Padding
          msgTx.buf[4] = 0x00;
          msgTx.buf[5] = 0x00; 
          msgTx.buf[6] = 0x00;
          msgTx.buf[7] = 0x00;
          msgTx.len = 8;            // number of bytes in request
          msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
      //    msgTx.flags.extended = 1; // 29 bit header, not 11 bit 
          msgTx.flags.remote = 0;
          
          msgTx.id = 0x7E0; // request header for PT           // Physical Addressing Request to Engine ECU
        //  msgTx.id = 0x18DB33F1; // request header for PT Functional Addressing request to all emission related ECUs.
          TFTCAN3.write(msgTx);
          Serial.println();
          Serial.print("canTx_PT_CAN_22_0x7E0_ECU_DTC_Clear_1_4_00 sent"); 
          Serial.println();

        // loopDelayPT_CAN_DTC_Clear = 0;
        }

    #pragma endregion

    #pragma region // Function - Flow Control

      void canTx_PT_CAN_22_0x700_FlowControl(){ //  function to manage flowcontrol buffers
        CAN_message_t msgTx, msgRx;
          msgTx.buf[0] = 0x30;     // Data Length 2byte
          msgTx.buf[1] = 0x0;     // 
          msgTx.buf[2] = 0x01;     // 
          msgTx.buf[3] = 0x55;        // Padding
          msgTx.buf[4] = 0x55;
          msgTx.buf[5] = 0x55;
          msgTx.buf[6] = 0x55;
          msgTx.buf[7] = 0x55;
          msgTx.len = 8;            // number of bytes in request
          // msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
        msgTx.flags.extended = 0; // 29 bit header, not 11 bit 
          msgTx.flags.remote = 0;


          msgTx.id = 0x700; // request header for PT           // Physical Addressing Request to Engine ECU
          // msgTx.id = 0x18DA11F1; // request header for PT Functional Addressing request to all emission related ECUs.
        //  msgTx.id = 0x18DB33F1; // request header for PT Functional Addressing request to all emission related ECUs.
          TFTCAN3.write(msgTx);
            PT_CAN_ECU_0x7E8_STAT_FC_22 = 2;  // Flow Control sent

            // Serial.println();
            // Serial.print("canTx_PT_CAN_22_0x700_PT_CAN_FlowControl REQ sent"); 
            // Serial.println();

        }

    #pragma endregion

    #pragma endregion

#pragma region // General PT-CAN ECU Keep-Awake canTx Functions

  void canTx_PT_CAN_ECU_GeneralKeepAwake_0x700_2_3E_80_55(){ // function to send keep-awake signals into PT_CAN
    CAN_message_t msgTx, msgRx;
      msgTx.buf[0] = 0x2;     // Data Length 2byte
      msgTx.buf[1] = 0x3E;     // 
      msgTx.buf[2] = 0x80;     // 
      msgTx.buf[3] = 0x55;        // Padding
      msgTx.buf[4] = 0x55;
      msgTx.buf[5] = 0x55; 
      msgTx.buf[6] = 0x55;
      msgTx.buf[7] = 0x55;
      msgTx.len = 8;            // number of bytes in request
      msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
  //    msgTx.flags.extended = 1; // 29 bit header, not 11 bit 
      msgTx.flags.remote = 0;
      
      msgTx.id = 0x700; // request header for PT_CAN           // Physical Addressing Request to Engine ECU
    //  msgTx.id = 0x18DB33F1; // request header for PT_CAN Functional Addressing request to all emission related ECUs.
      TFTCAN3.write(msgTx);

      // Serial.println();
      // Serial.print("canTx_PT_CAN_ECU_GeneralKeepAwake_0x700_2_3E_80_55 Sent  ");  Serial.print("\t"); 
      // Serial.println();



    }

  void canTx_PT_CAN_GeneralKeepAwake_0x700_2_3E_80_00(){ // function to send keep-awake signals into PT_CAN 
    CAN_message_t msgTx, msgRx;
      msgTx.buf[0] = 0x2;     // Data Length 2byte
      msgTx.buf[1] = 0x3E;     // 
      msgTx.buf[2] = 0x80;     // 
      msgTx.buf[3] = 0x00;        // Padding
      msgTx.buf[4] = 0x00;
      msgTx.buf[5] = 0x00; 
      msgTx.buf[6] = 0x00;
      msgTx.buf[7] = 0x00;
      msgTx.len = 8;            // number of bytes in request
      msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
  //    msgTx.flags.extended = 1; // 29 bit header, not 11 bit 
      msgTx.flags.remote = 0;
      
      msgTx.id = 0x700; // request header for PT_CAN           // Physical Addressing Request to Engine ECU
    //  msgTx.id = 0x18DB33F1; // request header for PT_CAN Functional Addressing request to all emission related ECUs.
      TFTCAN3.write(msgTx);

      Serial.println();
      Serial.print("canTx_PT_CAN_GeneralKeepAwake_0x700_2_3E_80_00 Sent  ");  Serial.print("\t"); 
      Serial.println();



    }

  void canTx_PT_CAN_ECU_GeneralKeepAwake_0x700_2_10_3_55(){ // function to send keep-awake signals into PT_CAN
    CAN_message_t msgTx, msgRx;
      msgTx.buf[0] = 0x2;     // Data Length 2byte
      msgTx.buf[1] = 0x10;     // 
      msgTx.buf[2] = 0x03;     // 
      msgTx.buf[3] = 0x55;        // Padding
      msgTx.buf[4] = 0x55;
      msgTx.buf[5] = 0x55; 
      msgTx.buf[6] = 0x55;
      msgTx.buf[7] = 0x55;
      msgTx.len = 8;            // number of bytes in request
      msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
  //    msgTx.flags.extended = 1; // 29 bit header, not 11 bit 
      msgTx.flags.remote = 0;
      
      msgTx.id = 0x700; // request header for PT_CAN           // Physical Addressing Request to Engine ECU
    //  msgTx.id = 0x18DB33F1; // request header for PT_CAN Functional Addressing request to all emission related ECUs.
      TFTCAN3.write(msgTx);
      
      Serial.println();
      Serial.print("canTx_PT_CAN_ECU_GeneralKeepAwake_0x700_2_10_3_55 Sent  ");  Serial.print("\t"); 
      Serial.println();



    }

  void canTx_PT_CAN_ECU_GeneralKeepAwake_0x700_2_10_3_00(){ // function to send keep-awake signals into PT_CAN
    CAN_message_t msgTx, msgRx;
      msgTx.buf[0] = 0x2;     // Data Length 2byte
      msgTx.buf[1] = 0x10;     // 
      msgTx.buf[2] = 0x03;     // 
      msgTx.buf[3] = 0x00;        // Padding
      msgTx.buf[4] = 0x00;
      msgTx.buf[5] = 0x00; 
      msgTx.buf[6] = 0x00;
      msgTx.buf[7] = 0x00;
      msgTx.len = 8;            // number of bytes in request
      msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
  //    msgTx.flags.extended = 1; // 29 bit header, not 11 bit 
      msgTx.flags.remote = 0;
      
      msgTx.id = 0x700; // request header for PT_CAN           // Physical Addressing Request to Engine ECU
    //  msgTx.id = 0x18DB33F1; // request header for PT_CAN Functional Addressing request to all emission related ECUs.
      TFTCAN3.write(msgTx);
      
      Serial.println();
      Serial.print("canTx_PT_CAN_ECU_GeneralKeepAwake_0x700_2_10_3_00 Sent  ");  Serial.print("\t"); 
      Serial.println();



    }

  void canTx_PT_CAN_ECU_GeneralKeepAwake_0x7E0_2_3E_00(){ // function to send keep-awake signals into PT_CAN
    CAN_message_t msgTx, msgRx;
      msgTx.buf[0] = 0x2;     // Data Length 2byte
      msgTx.buf[1] = 0x3E;     // 
      msgTx.buf[2] = 0x00;     // 
      msgTx.buf[3] = 0x00;        // Padding
      msgTx.buf[4] = 0x00;
      msgTx.buf[5] = 0x00; 
      msgTx.buf[6] = 0x00;
      msgTx.buf[7] = 0x00;
      msgTx.len = 8;            // number of bytes in request
      msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
  //    msgTx.flags.extended = 1; // 29 bit header, not 11 bit 
      msgTx.flags.remote = 0;
      
      msgTx.id = 0x7E0; // request header for PT_CAN           // Physical Addressing Request to Engine ECU
    //  msgTx.id = 0x18DB33F1; // request header for PT_CAN Functional Addressing request to all emission related ECUs.
      TFTCAN3.write(msgTx);

      Serial.println();
      Serial.print("canTx_PT_CAN_ECU_GeneralKeepAwake_0x7E0_2_3E_00 sent"); 
      Serial.println();



    }

  void canTx_PT_CAN_ECU_GeneralKeepAwake_0x7E0_2_10_3(){ // function to send keep-awake signals into PT_CAN
    CAN_message_t msgTx, msgRx;
      msgTx.buf[0] = 0x2;     // Data Length 2byte
      msgTx.buf[1] = 0x10;     // 
      msgTx.buf[2] = 0x03;     // 
      msgTx.buf[3] = 0x00;        // Padding
      msgTx.buf[4] = 0x00;
      msgTx.buf[5] = 0x00; 
      msgTx.buf[6] = 0x00;
      msgTx.buf[7] = 0x00;
      msgTx.len = 8;            // number of bytes in request
      msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
  //    msgTx.flags.extended = 1; // 29 bit header, not 11 bit 
      msgTx.flags.remote = 0;
      
      msgTx.id = 0x7E0; // request header for PT_CAN           // Physical Addressing Request to Engine ECU
    //  msgTx.id = 0x18DB33F1; // request header for PT_CAN Functional Addressing request to all emission related ECUs.
      TFTCAN3.write(msgTx);

      Serial.println();
      Serial.print("canTx_PT_CAN_ECU_GeneralKeepAwake_0x7E0_2_10_3 sent"); 
      Serial.println();



    }

  void canTx_PT_CAN_ECU_22_GeneralKeepAwake_0x713_3_22_F1(){ // function to send keep-awake signals into PT_CAN
    CAN_message_t msgTx, msgRx;
      msgTx.buf[0] = 0x3;     // Data Length 2byte
      msgTx.buf[1] = 0x22;     // 
      msgTx.buf[2] = 0xF1;     // 
      msgTx.buf[3] = 0x99;        // Padding
      msgTx.buf[4] = 0x55;
      msgTx.buf[5] = 0x55; 
      msgTx.buf[6] = 0x55;
      msgTx.buf[7] = 0x55;
      msgTx.len = 8;            // number of bytes in request
      msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
  //    msgTx.flags.extended = 1; // 29 bit header, not 11 bit 
      msgTx.flags.remote = 0;
      
      msgTx.id = 0x713; // request header for PT_CAN           // Physical Addressing Request to Engine ECU
    //  msgTx.id = 0x18DB33F1; // request header for PT_CAN Functional Addressing request to all emission related ECUs.
      TFTCAN3.write(msgTx);

      Serial.println();
      Serial.print("canTx_PT_CAN_ECU_22_GeneralKeepAwake_0x713_3_22_F1 Initial REQ sent"); 
      Serial.println();


    }

  void canTx_PT_CAN_ECU_22_GeneralKeepAwake_0x7E0_3_22_F1_99_55(){ // function to send keep-awake signals into PT_CAN
    CAN_message_t msgTx, msgRx;
      msgTx.buf[0] = 0x3;     // Data Length 2byte
      msgTx.buf[1] = 0x22;     // 
      msgTx.buf[2] = 0xF1;     // 
      msgTx.buf[3] = 0x99;        // Padding
      msgTx.buf[4] = 0x55;
      msgTx.buf[5] = 0x55; 
      msgTx.buf[6] = 0x55;
      msgTx.buf[7] = 0x55;
      msgTx.len = 8;            // number of bytes in request
      msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
  //    msgTx.flags.extended = 1; // 29 bit header, not 11 bit 
      msgTx.flags.remote = 0;
      
      msgTx.id = 0x7E0; // request header for PT_CAN           // Physical Addressing Request to Engine ECU
    //  msgTx.id = 0x18DB33F1; // request header for PT_CAN Functional Addressing request to all emission related ECUs.
      TFTCAN3.write(msgTx);

      Serial.println();
      Serial.print("canTx_PT_CAN_ECU_22_GeneralKeepAwake_0x7E0_3_22_F1_99_55 Initial REQ sent"); 
      Serial.println();

    
      }

 
#pragma endregion 

#pragma region // Relayed canTx messages via PT_CAN - Direct Standalone Suspension Level Sensors

  void canTx_PT_CAN_MasterInstruction_Susp_LvlSensorsToCAN_Reset() { //  
    CAN_message_t msgTx, msgRx;
    msgTx.buf[0] = 0x3;     // Data Length 2byte
    msgTx.buf[1] = 0x75;     // 
    msgTx.buf[2] = 0x1;     // 
    msgTx.buf[3] = 0x0;        // Padding
    msgTx.buf[4] = 0x0;
    msgTx.buf[5] = 0x0;
    msgTx.buf[6] = 0x0;
    msgTx.buf[7] = 0x0;
    msgTx.len = 8;            // number of bytes in request
    // msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
    msgTx.flags.extended = 0; // 29 bit header, not 11 bit 
    msgTx.flags.remote = 0;


    msgTx.id = 0x795; // request header for OBD           // Physical Addressing Request to Engine ECU
    // msgTx.id = 0x18DA11F1; // request header for OBD Functional Addressing request to all emission related ECUs.
  //  msgTx.id = 0x18DB33F1; // request header for OBD Functional Addressing request to all emission related ECUs.
    TFTCAN3.write(msgTx);
     Serial.print("canTx_PT_CAN_MasterInstruction_Susp_LvlSensorsToCAN_Reset REQ sent"); 
     Serial.println("");

  }

  void do_PT_CAN_Susp_Lvl_Sensors_Offset_Reset_DirectStandaloneModule() { // Overall Function for level sensor offset reset
    
        // Serial.print("PT_CAN_Resetting Offsets for SuspensionLevelSensors - DirectStandaloneModule..." );
        canTx_PT_CAN_MasterInstruction_Susp_LvlSensorsToCAN_Reset();

        Serial.println("");
        Serial.print("PT_CAN_Resetting Offsets for SuspensionLevelSensors - DirectStandaloneModule Completed..." );
        Serial.println("");
        
       }

#pragma endregion

#pragma region // canRx Services - Passive CAN Sniffing of messages from TFT Potentiometer Board (for TFT Standalone Haldex Controller)

  void canSniff0x792(const CAN_message_t &PT_CAN_msg0x792) //  function to sniff for A0,A1,A2,A3 from potentiometer board
  { 

     Serial.print("MB "); Serial.print(PT_CAN_msg0x792.mb);
     Serial.print(" ID: "); Serial.print(PT_CAN_msg0x792.id, HEX);
     Serial.print(" Buffer: ");
    for (uint8_t i = 0; i < PT_CAN_msg0x792.len; i++)
    {
         Serial.print(PT_CAN_msg0x792.buf[i], HEX); Serial.print(" ");
        // digitalToggle(LED_PIN_CANRecieve);
    }
          Serial.println("  ");


    PT_CAN_msg0x792_buf0_Raw = (PT_CAN_msg0x792.buf[0]);
    PT_CAN_msg0x792_buf1_Raw = (PT_CAN_msg0x792.buf[1]);
    PT_CAN_msg0x792_buf2_Raw = (PT_CAN_msg0x792.buf[2]);
    PT_CAN_msg0x792_buf3_Raw = (PT_CAN_msg0x792.buf[3]);
    PT_CAN_msg0x792_buf4_Raw = (PT_CAN_msg0x792.buf[4]);
    PT_CAN_msg0x792_buf5_Raw = (PT_CAN_msg0x792.buf[5]);
    PT_CAN_msg0x792_buf6_Raw = (PT_CAN_msg0x792.buf[6]);
    PT_CAN_msg0x792_buf7_Raw = (PT_CAN_msg0x792.buf[7]);


  }

  void canSniff0x793(const CAN_message_t &PT_CAN_msg0x793) // function to sniff for A4,A5,A8,A9 from potentiometer board
  { 

     Serial.print("MB "); Serial.print(PT_CAN_msg0x793.mb);
     Serial.print(" ID: "); Serial.print(PT_CAN_msg0x793.id, HEX);
     Serial.print(" Buffer: ");
    for (uint8_t i = 0; i < PT_CAN_msg0x793.len; i++)
    {
         Serial.print(PT_CAN_msg0x793.buf[i], HEX); Serial.print(" ");
        // digitalToggle(LED_PIN_CANRecieve);
    }
          Serial.println("  ");


    PT_CAN_msg0x793_buf0_Raw = (PT_CAN_msg0x793.buf[0]);
    PT_CAN_msg0x793_buf1_Raw = (PT_CAN_msg0x793.buf[1]);
    PT_CAN_msg0x793_buf2_Raw = (PT_CAN_msg0x793.buf[2]);
    PT_CAN_msg0x793_buf3_Raw = (PT_CAN_msg0x793.buf[3]);
    PT_CAN_msg0x793_buf4_Raw = (PT_CAN_msg0x793.buf[4]);
    PT_CAN_msg0x793_buf5_Raw = (PT_CAN_msg0x793.buf[5]);
    PT_CAN_msg0x793_buf6_Raw = (PT_CAN_msg0x793.buf[6]);
    PT_CAN_msg0x793_buf7_Raw = (PT_CAN_msg0x793.buf[7]);


  }




#pragma endregion

/////////////////////////////////////////////////

#pragma region // PT_CAN Sniffing and Service22 Requests - PT_CAN_TCU - 0x7E1/0x7E9 

  #pragma region // Switches to activate OEM Service Requests (Service01/Service09/Service22) via PT_CAN

  

    unsigned int Switch_Req_22_PT_CAN_TCU_AllTqsAndPressures;
    unsigned int Switch_Req_22_PT_CAN_TCU_Set1_ClutchPressureActuals;
    unsigned int Switch_Req_22_PT_CAN_TCU_Set2_ClutchPressureTargets;
    unsigned int Switch_Req_22_PT_CAN_TCU_Set3_ClutchPlateTempActuals;
    unsigned int Switch_Req_22_PT_CAN_TCU_Set4_ClutchTorqueActuals;
    unsigned int Switch_Req_22_PT_CAN_TCU_Set5_ClutchTorqueTargets;

  #pragma endregion

  #pragma region // Declarations for TCU Data Variables - (TCU OLD Set 1 - Pressure Actuals)

    elapsedMillis TimeSinceFirstResponseRecieved_TCU_Set1;
    elapsedMillis TimeSinceSTATFC1_PT_CAN_OLD_TCU_ReqSet1;
    elapsedMillis TimeSinceSTATFC2_PT_CAN_OLD_TCU_ReqSet1;
    bool PT_CAN_DQ500_DAQ_OLD_Set1_SequenceComplete;

    unsigned int PT_CAN_f_sid22_TCU_PressureActuals_multi;

    unsigned int PT_CAN_TCU_0x7E9_PressureActuals_CF_Max;
    unsigned int PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22;
    unsigned int PT_CAN_TCU_0x7E9_PressureActuals_seq_Num;
    unsigned int PT_CAN_TCU_0x7E9_PressureActuals_CF_Num;
    unsigned int PT_CAN_TCU_0x7E9_PressureActuals_f_sid22_multi;



    unsigned int PT_CAN_msg0x7E9_Clutch1_PressureActual_Raw, PT_CAN_msg0x7E9_Clutch2_PressureActual_Raw;
    unsigned int PT_CAN_msg0x7E9_Clutch1_PressureActual_Int, PT_CAN_msg0x7E9_Clutch2_PressureActual_Int;
    unsigned int PT_CAN_TCU_Clutch1_PressureActual_Int, PT_CAN_TCU_Clutch2_PressureActual_Int;
    unsigned int PT_CAN_TCU_OLD_Clutch1_PressureActual_Int, PT_CAN_TCU_OLD_Clutch2_PressureActual_Int;

    unsigned int PT_CAN_msg0x7E9_DQ500_OilPressureActual_Raw;
    unsigned int PT_CAN_msg0x7E9_DQ500_OilPressureActual_Int;
    unsigned int PT_CAN_TCU_DQ500_OilPressureActual_Int;
    unsigned int PT_CAN_TCU_OLD_DQ500_OilPressureActual_Int;

    unsigned char lsb_PT_CAN_TCU_Clutch1_PressureActual_Int, msb_PT_CAN_TCU_Clutch1_PressureActual_Int;
    unsigned char lsb_PT_CAN_TCU_Clutch2_PressureActual_Int, msb_PT_CAN_TCU_Clutch2_PressureActual_Int;
    unsigned char lsb_PT_CAN_TCU_DQ500_OilPressureActual_Int, msb_PT_CAN_TCU_DQ500_OilPressureActual_Int;

    // unsigned char lsb_PT_CAN_TCU_Clutch1_PressureActual_Int, msb_PT_CAN_TCU_Clutch1_PressureActual_Int;
    // unsigned char lsb_PT_CAN_TCU_Clutch2_PressureActual_Int, msb_PT_CAN_TCU_Clutch2_PressureActual_Int;
    // unsigned char lsb_PT_CAN_TCU_DQ500_OilPressureActual_Int, msb_PT_CAN_TCU_DQ500_OilPressureActual_Int;


    byte PT_CAN_msg0x7E9_Clutch1_PressureActual_LSB, PT_CAN_msg0x7E9_Clutch1_PressureActual_MSB;
    byte PT_CAN_msg0x7E9_Clutch2_PressureActual_LSB, PT_CAN_msg0x7E9_Clutch2_PressureActual_MSB;
    byte PT_CAN_msg0x7E9_DQ500_OilPressureActual_LSB, PT_CAN_msg0x7E9_DQ500_OilPressureActual_MSB;

  // byte msg0x7E9_Clutch1_PressureActual_LSB, msg0x7E9_Clutch1_PressureActual_MSB, msg0x7E9_Clutch2_PressureActual_LSB, msg0x7E9_Clutch2_PressureActual_MSB,  msg0x7E9_DQ500_OilPressureActual_LSB, msg0x7E9_DQ500_OilPressureActual_MSB;


  #pragma endregion





  #pragma region // CAN Sniffing Function - CatchAll -0x7E9

    void canSniffService22_PT_CAN_TCU_0x7E9_CatchAll(const CAN_message_t& PT_CAN_msg0x7E9) { // Sniffing of 0x7E9 - TCU (0x7E9 is a response to request sent to 0x7E1)
      // TCU Variables Identifiers
      // 10_2B - Clutch1 Friction Lining Temp
      // 10_2C - Clutch2 Friction Lining Temp
      // 10_2A - Clutch Center Plate temp
      // 10_D - Indicated Engine Torque w/o external interventions
      // 21_4 - DQ500 Oil Temp
      // 38_42 - DQ500 Oil Pressure Target
      // 38_1D - DQ500 Oil Pressure Actual
      // 38_2E - Clutch 1 Torque Target
      // 38_38 - Clutch 2 Torque Target
      // 38_2F - Clutch 1 Torque Actual
      // 38_39 - Clutch 2 Torque Actual
      // 38_EC - Clutch 1 Pressure Target
      // 38_EA - Clutch 1 Pressure Actual
      // 38_ED - Clutch 2 Pressure Target
      // 38_EB - Clutch 2 Pressure Actual


      //  Serial.println(" ");
      // Serial.print("PT_CAN_MB: ");
      // Serial.print(PT_CAN_msg0x7E9.mb); Serial.print("\t");
      // Serial.print("  ID: 0x");
      // Serial.print(PT_CAN_msg0x7E9.id, HEX); Serial.print("\t");
      // Serial.print("  EXT: ");
      // Serial.print(PT_CAN_msg0x7E9.flags.extended); Serial.print("\t");
      // Serial.print("  LEN: ");
      // Serial.print(PT_CAN_msg0x7E9.len); Serial.print("\t");
      // Serial.print(" DATA: ");

      for (uint8_t i = 0; i < 8; i++)
      {
        // Serial.print(PT_CAN_msg0x7E9.buf[i], HEX);
        // Serial.print(" ");
      }

        // Serial.println("");

    //  Serial.print("PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22: ");  Serial.print(PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22); Serial.println("\t");   


        PT_CAN_msg0x7E9_buf0_Raw = PT_CAN_msg0x7E9.buf[0];
        PT_CAN_msg0x7E9_buf1_Raw = PT_CAN_msg0x7E9.buf[1];
        PT_CAN_msg0x7E9_buf2_Raw = PT_CAN_msg0x7E9.buf[2];
        PT_CAN_msg0x7E9_buf3_Raw = PT_CAN_msg0x7E9.buf[3];
        PT_CAN_msg0x7E9_buf4_Raw = PT_CAN_msg0x7E9.buf[4];
        PT_CAN_msg0x7E9_buf5_Raw = PT_CAN_msg0x7E9.buf[5];
        PT_CAN_msg0x7E9_buf6_Raw = PT_CAN_msg0x7E9.buf[6];
        PT_CAN_msg0x7E9_buf7_Raw = PT_CAN_msg0x7E9.buf[7];

  //                   Serial.print(PT_CAN_msg0x7E9_buf0_Raw, HEX);     Serial.print(" ");
  //                   Serial.print(PT_CAN_msg0x7E9_buf1_Raw, HEX);     Serial.print(" ");
  //                   Serial.print(PT_CAN_msg0x7E9_buf2_Raw, HEX);     Serial.print(" ");
  //                   Serial.print(PT_CAN_msg0x7E9_buf3_Raw, HEX);     Serial.print(" ");
  //                   Serial.print(PT_CAN_msg0x7E9_buf4_Raw, HEX);     Serial.print(" ");
  //                   Serial.print(PT_CAN_msg0x7E9_buf5_Raw, HEX);     Serial.print(" ");
  //                   Serial.print(PT_CAN_msg0x7E9_buf6_Raw, HEX);     Serial.print(" ");
  //                   Serial.print(PT_CAN_msg0x7E9_buf7_Raw, HEX);     Serial.print(" ");


      #pragma region // Recognize 0x7E9 Response Frames, coordinate buffered messages

          if (PT_CAN_msg0x7E9_buf0_Raw == 0x03 && PT_CAN_msg0x7E9_buf1_Raw == 0x6e){    ////////// Positive Response message  and First Frame.
                    PT_CAN_CF_Max = 1;
                    PT_CAN_f_sid22_0x7E9_multi = 1;   //
                      
                    if(PT_CAN_msg0x7E9_buf2_Raw == 0xF1){   // Means Info Type == 02 VIN. InfoType == 04 PT_CAN_CALID.
                        PT_CAN_msg0x7E9_x1buf4_Raw = PT_CAN_msg0x7E9_buf4_Raw;
                        PT_CAN_msg0x7E9_x1buf5_Raw = PT_CAN_msg0x7E9_buf5_Raw;
                        PT_CAN_msg0x7E9_x1buf6_Raw = PT_CAN_msg0x7E9_buf6_Raw;
                        PT_CAN_msg0x7E9_x1buf7_Raw = PT_CAN_msg0x7E9_buf7_Raw;  
                        PT_CAN_STAT_FC_22_0x7E9 = 1; 
                      
                        // Serial.print(PT_CAN_msg0x7E9.buf[0], HEX);     Serial.print(" ");
                        // Serial.print(PT_CAN_msg0x7E9.buf[1], HEX);     Serial.print(" ");
                        // Serial.print(PT_CAN_msg0x7E9.buf[2], HEX);     Serial.print(" ");
                        // Serial.print(PT_CAN_msg0x7E9.buf[3], HEX);     Serial.print(" ");
                        // Serial.print(PT_CAN_msg0x7E9.buf[4], HEX);     Serial.print(" ");
                        // Serial.print(PT_CAN_msg0x7E9.buf[5], HEX);     Serial.print(" ");
                        // Serial.print(PT_CAN_msg0x7E9.buf[6], HEX);     Serial.print(" ");
                        // Serial.print(PT_CAN_msg0x7E9.buf[7], HEX);     Serial.print(" ");

                    }
                  }
            else if(PT_CAN_f_sid22_0x7E9_multi ==1 && PT_CAN_msg0x7E9_buf0_Raw >20 && PT_CAN_STAT_FC_22_0x7E9 ==2 ){     // Means CF: Consecutive Frame and Buf[0] is sequence number
                PT_CAN_seq_Num = PT_CAN_msg0x7E9_buf0_Raw; //
                        //  Serial.print("Starting ElseIF  ");

                        // Serial.print(PT_CAN_msg0x7E9.buf[0], HEX);     Serial.print(" ");
                        // Serial.print(PT_CAN_msg0x7E9.buf[1], HEX);     Serial.print(" ");
                        // Serial.print(PT_CAN_msg0x7E9.buf[2], HEX);     Serial.print(" ");
                        // Serial.print(PT_CAN_msg0x7E9.buf[3], HEX);     Serial.print(" ");
                        // Serial.print(PT_CAN_msg0x7E9.buf[4], HEX);     Serial.print(" ");
                        // Serial.print(PT_CAN_msg0x7E9.buf[5], HEX);     Serial.print(" ");
                        // Serial.print(PT_CAN_msg0x7E9.buf[6], HEX);     Serial.print(" ");
                        // Serial.print(PT_CAN_msg0x7E9.buf[7], HEX);     Serial.print(" ");

                    if(PT_CAN_seq_Num == 0x21){   // Sequence Number is 1.
                        PT_CAN_msg0x7E9_x21buf1_Raw  = PT_CAN_msg0x7E9_buf1_Raw;
                        PT_CAN_msg0x7E9_x21buf2_Raw  = PT_CAN_msg0x7E9_buf2_Raw;
                        PT_CAN_msg0x7E9_x21buf3_Raw  = PT_CAN_msg0x7E9_buf3_Raw;
                        PT_CAN_msg0x7E9_x21buf4_Raw  = PT_CAN_msg0x7E9_buf4_Raw;
                        PT_CAN_msg0x7E9_x21buf5_Raw  = PT_CAN_msg0x7E9_buf5_Raw;
                        PT_CAN_msg0x7E9_x21buf6_Raw  = PT_CAN_msg0x7E9_buf6_Raw;
                        PT_CAN_msg0x7E9_x21buf7_Raw  = PT_CAN_msg0x7E9_buf7_Raw; // last byte not used
                        PT_CAN_CF_Num = 1;
                        // Serial.print("CFNum = 1");
                      }

                      if(PT_CAN_CF_Num == PT_CAN_CF_Max){
                        if(PT_CAN_STAT_FC_22_0x7E9 == 2){
                            PT_CAN_STAT_FC_22_0x7E9 = 0;
                          // Serial.print("CFMax: ");

                            PT_CAN_CF_Num = 0;
                            PT_CAN_f_sid22_0x7E9_multi = 0;   
                                    //  Serial.print("CFMax: ");
                                    //  Serial.print(CFNum);
                          }
                        }
                    }

        #pragma endregion

        //  PT_CAN_msg0x7E9_buf0_Raw = PT_CAN_msg0x7E9.buf[0];
        //  PT_CAN_msg0x7E9_buf1_Raw = PT_CAN_msg0x7E9.buf[1];
        //  PT_CAN_msg0x7E9_buf2_Raw = PT_CAN_msg0x7E9.buf[2];
        //  PT_CAN_msg0x7E9_buf3_Raw = PT_CAN_msg0x7E9.buf[3];
        //  PT_CAN_msg0x7E9_buf4_Raw = PT_CAN_msg0x7E9.buf[4];
        //  PT_CAN_msg0x7E9_buf5_Raw = PT_CAN_msg0x7E9.buf[5];
        //  PT_CAN_msg0x7E9_buf6_Raw = PT_CAN_msg0x7E9.buf[6];
        //  PT_CAN_msg0x7E9_buf7_Raw = PT_CAN_msg0x7E9.buf[7];


        }


#pragma endregion



  #pragma region // canTx Functions - DAQ and DQ500 Resets

    #pragma region // canTX Functions for DQ500 Continous DAQ - Old Set 1 - Pressures Only

      void canTx_PT_CAN_22_TCU_OLD_Set1_PressureActualsOnly(){ // function to request TCU Pressure Actuals via OBD Service 22 Request
        CAN_message_t msgTx, msgRx;
          msgTx.buf[0] = 0x7;     // Data Length 2byte
          msgTx.buf[1] = 0x22;     // 
          msgTx.buf[2] = 0x38;     // 
          msgTx.buf[3] = 0xEB;        // Padding
          msgTx.buf[4] = 0x38;
          msgTx.buf[5] = 0xEA; 
          msgTx.buf[6] = 0x38;
          msgTx.buf[7] = 0x1D;
          msgTx.len = 8;            // number of bytes in request
          msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
      //    msgTx.flags.extended = 1; // 29 bit header, not 11 bit 
          msgTx.flags.remote = 0;
          
          msgTx.id = 0x7E1; // request header for OBD           // Physical Addressing Request to Engine ECU
          TFTCAN3.write(msgTx);
        //  Serial.print("TCUSet1InitialREQ sent on PT_CAN..."); 

        //  Serial.print("TCUSet1InitialREQ sent on PT_CAN...  ");   Serial.println("\t");    
        // Serial.print("PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22: ");  Serial.print(PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22); Serial.println("\t");   

        }

      void canTx_PT_CAN_22_TCU_OLD_Set1_PressureActualsOnly_FlowControl(){ //  function to manage flowcontrol buffers via OBD Service 09 Request
        CAN_message_t msgTx, msgRx;
          msgTx.buf[0] = 0x30;     // Data Length 2byte
          msgTx.buf[1] = 0x0;     // 
          msgTx.buf[2] = 0x01;     // 
          msgTx.buf[3] = 0x00;        // Padding
          msgTx.buf[4] = 0x00;
          msgTx.buf[5] = 0x00;
          msgTx.buf[6] = 0x00;
          msgTx.buf[7] = 0x00;
          msgTx.len = 8;            // number of bytes in request
          // msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
        msgTx.flags.extended = 0; // 29 bit header, not 11 bit 
          msgTx.flags.remote = 0;


          msgTx.id = 0x7E1; // request header for OBD           // Physical Addressing Request to Engine ECU
          // if(DS1_ECU_LoggingConfigActive > 8000) {
          TFTCAN3.write(msgTx);
          // }
          PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22 = 2;  // Flow Control sent

          TimeSinceSTATFC1_PT_CAN_OLD_TCU_ReqSet1 = 0;
      
        //  Serial.print("TCUSet1FCmessage sent: ");    Serial.println("\t");    
        //  Serial.print("TimeSinceSTATFC1_PT_CAN_OLD_TCU_ReqSet1 Sent: ");  Serial.print(TimeSinceSTATFC1_PT_CAN_OLD_TCU_ReqSet1); Serial.println("\t");   
        // Serial.print("PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22: ");  Serial.print(PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22); Serial.println("\t");   

        //  Serial.print(" "); 
        
        }

    #pragma endregion


    #pragma region // canTx Functions for DQ500 Resets

      void canTx_PT_CAN_22_0x7E1_DQ500_ClutchCharacteristicCurve_Reset(){ // function to Reset DQ500 Clutch Characteristic Curve 
        CAN_message_t msgTx, msgRx;
          msgTx.buf[0] = 0x7;     // Data Length 2byte
          msgTx.buf[1] = 0x31;     // 
          msgTx.buf[2] = 0x1;     // 
          msgTx.buf[3] = 0x3;        // Padding
          msgTx.buf[4] = 0x86;
          msgTx.buf[5] = 0x4; 
          msgTx.buf[6] = 0x0;
          msgTx.buf[7] = 0x0;
          msgTx.len = 8;            // number of bytes in request
          msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
      //    msgTx.flags.extended = 1; // 29 bit header, not 11 bit 
          msgTx.flags.remote = 0;
          
          msgTx.id = 0x7E1; // request header for PT           // Physical Addressing Request to Engine ECU
        //  msgTx.id = 0x18DB33F1; // request header for PT Functional Addressing request to all emission related ECUs.
          TFTCAN3.write(msgTx);
        Serial.print("canTx_PT_CAN_22_0x7E1_DQ500_ClutchCharacteristicCurve_Reset Initial REQ sent"); 
        }

      void canTx_PT_CAN_22_0x7E1_DQ500_DblClutchFastAdaptation_Reset(){ // function to Reset DQ500 DblClutchFastAdaptation
        CAN_message_t msgTx, msgRx;
          msgTx.buf[0] = 0x7;     // Data Length 2byte
          msgTx.buf[1] = 0x31;     // 
          msgTx.buf[2] = 0x1;     // 
          msgTx.buf[3] = 0x3;        // Padding
          msgTx.buf[4] = 0x89;
          msgTx.buf[5] = 0x4; 
          msgTx.buf[6] = 0x0;
          msgTx.buf[7] = 0x0;
          msgTx.len = 8;            // number of bytes in request
          msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
      //    msgTx.flags.extended = 1; // 29 bit header, not 11 bit 
          msgTx.flags.remote = 0;
          
          msgTx.id = 0x7E1; // request header for PT           // Physical Addressing Request to Engine ECU
        //  msgTx.id = 0x18DB33F1; // request header for PT Functional Addressing request to all emission related ECUs.
          TFTCAN3.write(msgTx);
        Serial.print("canTx_PT_CAN_22_0x7E1_DQ500_DblClutchFastAdaptation_Reset Initial REQ sent"); 
        }

      void canTx_PT_CAN_22_0x7E1_DQ500_BasicSettings_Reset(){ // function to Reset Basic Settings
        CAN_message_t msgTx, msgRx;
          msgTx.buf[0] = 0x7;     // Data Length 2byte
          msgTx.buf[1] = 0x31;     // 
          msgTx.buf[2] = 0x1;     // 
          msgTx.buf[3] = 0x3;        // Padding
          msgTx.buf[4] = 0x80;
          msgTx.buf[5] = 0x4; 
          msgTx.buf[6] = 0x0;
          msgTx.buf[7] = 0x0;
          msgTx.len = 8;            // number of bytes in request
          msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
      //    msgTx.flags.extended = 1; // 29 bit header, not 11 bit 
          msgTx.flags.remote = 0;
          
          msgTx.id = 0x7E1; // request header for PT           // Physical Addressing Request to Engine ECU
        //  msgTx.id = 0x18DB33F1; // request header for PT Functional Addressing request to all emission related ECUs.
          TFTCAN3.write(msgTx);
        Serial.print("canTx_PT_CAN_22_0x7E1_DQ500_BasicSettings_Reset Initial REQ sent"); 
        }

      void canTx_PT_CAN_22_0x7E1_DQ500_FlowControl(){ //  function to manage flowcontrol buffers
        CAN_message_t msgTx, msgRx;
          msgTx.buf[0] = 0x30;     // Data Length 2byte
          msgTx.buf[1] = 0x0;     // 
          msgTx.buf[2] = 0x01;     // 
          msgTx.buf[3] = 0x55;        // Padding
          msgTx.buf[4] = 0x55;
          msgTx.buf[5] = 0x55;
          msgTx.buf[6] = 0x55;
          msgTx.buf[7] = 0x55;
          msgTx.len = 8;            // number of bytes in request
          // msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
        msgTx.flags.extended = 0; // 29 bit header, not 11 bit 
          msgTx.flags.remote = 0;


          msgTx.id = 0x7E1; // request header for PT           // Physical Addressing Request to Engine ECU
          // msgTx.id = 0x18DA11F1; // request header for PT Functional Addressing request to all emission related ECUs.
        //  msgTx.id = 0x18DB33F1; // request header for PT Functional Addressing request to all emission related ECUs.
          TFTCAN3.write(msgTx);
            PT_CAN_STAT_FC_22_0x7E9 = 2;  // Flow Control sent
        Serial.print("canTx_PT_CAN_22_0x7E1_DQ500_FlowControl REQ sent"); 
        
        }

    #pragma endregion

    
  #pragma endregion




#pragma endregion

/////////////////////////////////////////////////

#pragma region // CAN Sniffing of PT_CAN 0x7E2 to recognize when DS1 is attempting to connect to/configure ECU datalogging

  void canSniffService22_PT_CAN_ECU_0x7E2(const CAN_message_t& PT_CAN_msg0x7E2) { // Sniffing of 0x7EA (0x7EA is active when DS1 is configuring ECU for datalogging)

    //  Serial.println(" ");
    //  Serial.print("MB: ");
    //  Serial.print(PT_CAN_msg0x7E2.mb);
    //  Serial.print("  ID: 0x");
    //  Serial.print(PT_CAN_msg0x7E2.id, HEX);
    //  Serial.print("  EXT: ");
    //  Serial.print(PT_CAN_msg0x7E2.flags.extended);
    //  Serial.print("  LEN: ");
    //  Serial.print(PT_CAN_msg0x7E2.len);
    //  Serial.print(" DATA: ");
    for (uint8_t i = 0; i < 8; i++)
    {
      //  Serial.print(PT_CAN_msg0x7E2.buf[i], HEX);
      //  Serial.print(" ");
    }

    // Serial.println("");

    // Serial.println("");
    // Serial.println("DS1 Configuring ECU for Datalogging....   ");


    if((PT_CAN_msg0x7E2.buf[0] == 0x1) && (PT_CAN_msg0x7E2.buf[3] == 0xAD)) {
    Serial.println("");
    Serial.println("DS1 Configuring ECU for Datalogging....Detected via PT_CAN_0x7E2   ");
    // PT_CAN_DQ500_DAQ_OLD_Set1_SequenceComplete == true;
    
DS1_ECU_LoggingConfigActive_Int = 1;
  // PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22 = 0;

    }

    if( (PT_CAN_msg0x7E2.buf[0] > 0x1)) {
    // else {
  DS1_ECU_LoggingConfigActive_Int = 0;
  // PT_CAN_DQ500_DAQ_OLD_Set1_SequenceComplete = true;
  // PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22 = 0;
}

    if( DS1_ECU_LoggingConfigActive_Int == 1) {   
          // PT_CAN_DQ500_DAQ_OLD_Set1_SequenceComplete = false;
  // PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22 = 0;

      DS1_ECU_LoggingConfigActive = 0;  // Resets timer 
    }
// else {
//   DS1_ECU_LoggingConfigActive == 0;
// }
//   }

  }

#pragma endregion 





#pragma endregion
// -------------------------------------------------------------------------------------------------------------------------------------


#pragma region // canTx Functions - Executable Momentary Paddle Signal Generations

  void canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(){ // function to inject the Paddle UP Signal to 0x3DD, along with the current (most recently read) bytes in the 0x3DD frame
    CAN_message_t msgTx, msgRx;
      msgTx.buf[0] = PT_CAN_msg0x3DD_buf0;     // Data Length 2byte
      msgTx.buf[1] = PT_CAN_msg0x3DD_buf1;     // 
      msgTx.buf[2] = PT_CAN_msg0x3DD_buf2;   // 
      msgTx.buf[3] = PT_CAN_msg0x3DD_buf3;        // Padding
      msgTx.buf[4] = PT_CAN_msg0x3DD_buf4;
      msgTx.buf[5] = PT_CAN_msg0x3DD_buf5; 
      msgTx.buf[6] = PT_CAN_msg0x3DD_buf6;
      msgTx.buf[7] = 02;
      msgTx.len = 8;            // number of bytes in request
      msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
  //    msgTx.flags.extended = 1; // 29 bit header, not 11 bit 
      msgTx.flags.remote = 0;
      
      msgTx.id = 0x3DD; // request header for PT_CAN           // Physical Addressing Request to Engine ECU
    //  msgTx.id = 0x18DB33F1; // request header for PT_CAN Functional Addressing request to all emission related ECUs.
      TFTCAN3.write(msgTx);
     Serial.print("canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration CAN Message Sent...");      Serial.println(" "); 

    }

  void canTx_PT_MQB_0x3DD_PaddleDOWN_SignalGeneration(){ // function to inject the Paddle DOWN Signal to 0x3DD, along with the current (most recently read) bytes in the 0x3DD frame 
    CAN_message_t msgTx, msgRx;
      msgTx.buf[0] = PT_CAN_msg0x3DD_buf0;     // Data Length 2byte
      msgTx.buf[1] = PT_CAN_msg0x3DD_buf1;     // 
      msgTx.buf[2] = PT_CAN_msg0x3DD_buf2;   // 
      msgTx.buf[3] = PT_CAN_msg0x3DD_buf3;        // Padding
      msgTx.buf[4] = PT_CAN_msg0x3DD_buf4;
      msgTx.buf[5] = PT_CAN_msg0x3DD_buf5; 
      msgTx.buf[6] = PT_CAN_msg0x3DD_buf6;
      msgTx.buf[7] = 01;
      msgTx.len = 8;            // number of bytes in request
      msgTx.flags.extended = 0; // 11 bit header, not 29 bit 
  //    msgTx.flags.extended = 1; // 29 bit header, not 11 bit 
      msgTx.flags.remote = 0;
      
      msgTx.id = 0x3DD; // request header for PT_CAN           // Physical Addressing Request to Engine ECU
    //  msgTx.id = 0x18DB33F1; // request header for PT_CAN Functional Addressing request to all emission related ECUs.
      TFTCAN3.write(msgTx);

     Serial.print("canTx_PT_MQB_0x3DD_PaddleDOWN_SignalGeneration CAN Message sent...");      Serial.println(""); 

    }

  #pragma endregion



void TFTCAN_PT_CAN_VehicleIdentification() {

  // TFT_VehicleIdentificationComplete == 0;

  // Serial.println("");
  // Serial.print("Beginning of PT_CAN Vehicle Identification  Function..."); Serial.println("");
  // Serial.println("");


  if( loopDelayPT_CAN_Service09 > 250 && PT_CAN_STAT_FC_09 == 0 ) { 

    if( PT_CAN_VIN_ReadComplete == 0 )  { // Read VIN

      // Serial.println("");
      // Serial.print("Beginning VIN Read Function..."); Serial.println("");
      // Serial.println("");
    

        canTx_PT_CAN_09_Identification_VIN();
        loopDelayPT_CAN_Service09 = 0;
        // PT_CAN_VIN_ReadComplete = 1;

        }

    else if( PT_CAN_VIN_ReadComplete == 1 
        && PT_CAN_ECU_HW_ReadComplete == 0  ) { // Read ECU HW ID

        // Serial.println("");
        // Serial.print("Beginning ECU HW ID Read Function..."); Serial.println("");
        // Serial.println("");
    
        canTx_PT_CAN_09_Identification_ECU_HW();
        loopDelayPT_CAN_Service09 = 0;
        // PT_CAN_ECU_HW_ReadComplete = 1;
        }
  

    else if( PT_CAN_VIN_ReadComplete == 1 
        && PT_CAN_ECU_HW_ReadComplete == 1
        && PT_CAN_ECU_CALID_ReadComplete == 0  ) { // Read ECU PT_CAN_CAL ID
    
        // Serial.println("");
        // Serial.print("Beginning ECU PT_CAN_CALID ID Read Function..."); Serial.println("");
        // Serial.println("");
  
        canTx_PT_CAN_09_Identification_ECU_CALID();
        loopDelayPT_CAN_Service09 = 0;
        // PT_CAN_ECU_CALID_ReadComplete = 1;
        }


    else if( PT_CAN_VIN_ReadComplete == 1 
         && PT_CAN_ECU_HW_ReadComplete == 1
         && PT_CAN_ECU_CALID_ReadComplete == 1
         && PT_CAN_ECU_Variable4_ReadComplete == 2  ) { // Read ECU_Variable4
    
         Serial.println("");
         Serial.print("Beginning ECU_Variable4 Read Function..."); Serial.println("");
         Serial.println("");
  
         canTx_PT_CAN_09_Identification_ECU_Variable4();
    
         loopDelayPT_CAN_Service09 = 0;
         PT_CAN_ECU_Variable4_ReadComplete = 1;
        }
  
        
    else if( PT_CAN_VIN_ReadComplete == 1 
         && PT_CAN_ECU_HW_ReadComplete == 1
         && PT_CAN_ECU_CALID_ReadComplete == 1
         && PT_CAN_ECU_Variable4_ReadComplete == 2
         && PT_CAN_ECU_Variable5_ReadComplete == 2  ) { // Read ECU_Variable5

         Serial.println("");
         Serial.print("Beginning ECU_Variable5 Read Function..."); Serial.println("");
         Serial.println("");

         canTx_PT_CAN_09_Identification_ECU_Variable5();
  
         loopDelayPT_CAN_Service09 = 0;
         PT_CAN_ECU_Variable5_ReadComplete = 1;
         }
      
    else if( PT_CAN_VIN_ReadComplete == 1 
         && PT_CAN_ECU_HW_ReadComplete == 1
         && PT_CAN_ECU_CALID_ReadComplete == 1
         && PT_CAN_ECU_Variable4_ReadComplete == 2 
         && PT_CAN_ECU_Variable5_ReadComplete == 2                   
         && PT_CAN_ECU_Variable6_ReadComplete == 2                   
         && PT_CAN_ECU_Variable7_ReadComplete == 2  ) { // Read ECU_Variable6
       
         Serial.println("");
         Serial.print("Beginning ECU_Variable6 Read Function..."); Serial.println("");
         Serial.println("");

         canTx_PT_CAN_09_Identification_ECU_Variable4();
   
         loopDelayPT_CAN_Service09 = 0;
         PT_CAN_ECU_Variable6_ReadComplete = 1;

        }     
            
    else if( PT_CAN_VIN_ReadComplete == 1 
         && PT_CAN_ECU_HW_ReadComplete == 1
         && PT_CAN_ECU_CALID_ReadComplete == 1
         && PT_CAN_ECU_Variable4_ReadComplete == 2 
         && PT_CAN_ECU_Variable5_ReadComplete == 2                   
         && PT_CAN_ECU_Variable6_ReadComplete == 2                   
         && PT_CAN_ECU_Variable7_ReadComplete == 2  ) { // Read ECU_Variable7
    
         Serial.println("");
         Serial.print("Beginning ECU_Variable7 Read Function..."); Serial.println("");
         Serial.println("");
    
         canTx_PT_CAN_09_Identification_ECU_Variable7();
  
         loopDelayPT_CAN_Service09 = 0;
         PT_CAN_ECU_Variable7_ReadComplete = 1;

         }
    
        
  
    else if( PT_CAN_VIN_ReadComplete == 1 
         && PT_CAN_ECU_HW_ReadComplete == 1
         && PT_CAN_ECU_CALID_ReadComplete == 1
         && PT_CAN_TCU_Variable1_ReadComplete == 1
         && PT_CAN_TCU_Variable2_ReadComplete == 2
         && PT_CAN_TCU_Variable3_ReadComplete == 3  ) { // Read TCU_Variable1

         Serial.println("");
         Serial.print("Beginning TCU_Variable1 Read Function..."); Serial.println("");
         Serial.println("");
  
         canTx_PT_CAN_09_Identification_TCU_Variable1();
    
         loopDelayPT_CAN_Service09 = 0;
         PT_CAN_TCU_Variable1_ReadComplete = 1;
        }

        
    else if( PT_CAN_VIN_ReadComplete == 1 
         && PT_CAN_ECU_HW_ReadComplete == 1
         && PT_CAN_ECU_CALID_ReadComplete == 1
         && PT_CAN_TCU_Variable1_ReadComplete == 1
         && PT_CAN_TCU_Variable2_ReadComplete == 2
         && PT_CAN_TCU_Variable3_ReadComplete == 3  ) { // Read TCU_Variable2
 
         Serial.println("");
         Serial.print("Beginning TCU_Variable2 Read Function..."); Serial.println("");
         Serial.println("");
   
         canTx_PT_CAN_09_Identification_TCU_Variable2();
     
         loopDelayPT_CAN_Service09 = 0;
         PT_CAN_TCU_Variable2_ReadComplete = 1;

         }
   
   
    else if( PT_CAN_VIN_ReadComplete == 1 
         && PT_CAN_ECU_HW_ReadComplete == 1
         && PT_CAN_ECU_CALID_ReadComplete == 1
         && PT_CAN_TCU_Variable1_ReadComplete == 1
         && PT_CAN_TCU_Variable2_ReadComplete == 2
         && PT_CAN_TCU_Variable3_ReadComplete == 3  ) { // Read TCU_Variable3
    
         Serial.println("");
         Serial.print("Beginning TCU_Variable3 Read Function..."); Serial.println("");
         Serial.println("");
  
         canTx_PT_CAN_09_Identification_TCU_Variable3();
    
         loopDelayPT_CAN_Service09 = 0;
         PT_CAN_TCU_Variable3_ReadComplete = 1;
         }
        

        
    if(PT_CAN_STAT_FC_09 ==1){

          delay(2);
          canTx_PT_CAN_09_FlowControl();
      
      
      }
 


      if(PT_CAN_ECU_CALID_ReadComplete == 1 && TFT_VehicleIdentificationComplete == 0 ) {
      
        TFT_VehicleIdentificationComplete = 1;

          // Serial.println("");
          // Serial.print("PT_CAN_VIN_ReadComplete: "); Serial.print(PT_CAN_VIN_ReadComplete); Serial.print("\t");
          // Serial.print("PT_CAN_ECU_HW_ReadComplete: "); Serial.print(PT_CAN_ECU_HW_ReadComplete); Serial.print("\t");
          // Serial.print("PT_CAN_ECU_CALID_ReadComplete: "); Serial.print(PT_CAN_ECU_CALID_ReadComplete); Serial.print("\t");
          // Serial.print("TFT_VehicleIdentificationComplete: "); Serial.print(TFT_VehicleIdentificationComplete); Serial.print("\t");
          // Serial.println("");


          Serial.print("VIN: ");
          Serial.print (PT_CAN_VIN_buf1);
          Serial.print (PT_CAN_VIN_buf2);
          Serial.print (PT_CAN_VIN_buf3);
          Serial.print (PT_CAN_VIN_buf4);
          Serial.print (PT_CAN_VIN_buf5);
          Serial.print (PT_CAN_VIN_buf6);
          Serial.print (PT_CAN_VIN_buf7);
          Serial.print (PT_CAN_VIN_buf8);
          Serial.print (PT_CAN_VIN_buf9);
          Serial.print (PT_CAN_VIN_buf10);
          Serial.print (PT_CAN_VIN_buf11);
          Serial.print (PT_CAN_VIN_buf12);
          Serial.print (PT_CAN_VIN_buf13);
          Serial.print (PT_CAN_VIN_buf14);
          Serial.print (PT_CAN_VIN_buf15);
          Serial.print (PT_CAN_VIN_buf16);
          Serial.print (PT_CAN_VIN_buf17);
          // Serial.print(",");
          // Serial.print("endbuffer"); Serial.print("\t");
          Serial.println("");



          Serial.print("Module Name: (Generic)"); Serial.print("\t");
          Serial.print(PT_CAN_ECUName_buf1); 
          Serial.print(PT_CAN_ECUName_buf2); 
          Serial.print(PT_CAN_ECUName_buf3); 
          Serial.print(PT_CAN_ECUName_buf4); 
          Serial.print(PT_CAN_ECUName_buf5); 
          Serial.print(PT_CAN_ECUName_buf6); 
          Serial.print(PT_CAN_ECUName_buf7); 
          Serial.print(PT_CAN_ECUName_buf8); 
          Serial.print(PT_CAN_ECUName_buf9); 
          Serial.print(PT_CAN_ECUName_buf10); 
          Serial.print(PT_CAN_ECUName_buf11); 
          Serial.print(PT_CAN_ECUName_buf12); 
          Serial.print(PT_CAN_ECUName_buf13); 
          Serial.print(PT_CAN_ECUName_buf14); 
          Serial.print(PT_CAN_ECUName_buf15); 
          Serial.print(PT_CAN_ECUName_buf16); 
          Serial.print(PT_CAN_ECUName_buf17); 
          Serial.print(PT_CAN_ECUName_buf18); 
          Serial.print(PT_CAN_ECUName_buf19); 
          Serial.print(PT_CAN_ECUName_buf20); 
          Serial.print(PT_CAN_ECUName_buf21); 
          Serial.print(PT_CAN_ECUName_buf22); 
          Serial.print(PT_CAN_ECUName_buf23); 
          Serial.print(PT_CAN_ECUName_buf24); 
          // Serial.print(",");
          // Serial.print("endbuffer"); Serial.print("\t");
          Serial.println("");

          Serial.print("PT_CAN_ECU PT_CAN_CALID: "); 
          Serial.print(PT_CAN_CALID_buf1); 
          Serial.print(PT_CAN_CALID_buf2); 
          Serial.print(PT_CAN_CALID_buf3); 
          Serial.print(PT_CAN_CALID_buf4); 
          Serial.print(PT_CAN_CALID_buf5); 
          Serial.print(PT_CAN_CALID_buf6); 
          Serial.print(PT_CAN_CALID_buf7); 
          Serial.print(PT_CAN_CALID_buf8); 
          Serial.print(PT_CAN_CALID_buf9); 
          Serial.print(PT_CAN_CALID_buf10); 
          Serial.print(PT_CAN_CALID_buf11); 
          Serial.print(PT_CAN_CALID_buf12); 
          Serial.print(PT_CAN_CALID_buf13); 
          Serial.print(PT_CAN_CALID_buf14); 
          Serial.print(PT_CAN_CALID_buf15); 
          Serial.print(PT_CAN_CALID_buf16); 
          Serial.print(PT_CAN_CALID_buf17); 
          // Serial.print(",");
          // Serial.print("endbuffer"); Serial.print("\t");
          Serial.println("");
          Serial.println("");


          // Serial.println("");
          // Serial.print("PT_CAN_VIN_ReadComplete: "); Serial.print(PT_CAN_VIN_ReadComplete); Serial.print("\t");
          // Serial.print("PT_CAN_ECU_HW_ReadComplete: "); Serial.print(PT_CAN_ECU_HW_ReadComplete); Serial.print("\t");
          // Serial.print("PT_CAN_ECU_CALID_ReadComplete: "); Serial.print(PT_CAN_ECU_CALID_ReadComplete); Serial.print("\t");
          // Serial.print("TFT_VehicleIdentificationComplete: "); Serial.print(TFT_VehicleIdentificationComplete); Serial.print("\t");
          // Serial.println("");


      }
  }

  // Serial.println("");
  // Serial.print("End of PT_CAN Vehicle Identification Function..."); Serial.println("");
  // Serial.println("");

  

}


  

void TFTCAN_PT_CAN_init(){

  Serial.println("");
  Serial.print("Initializing TFTCAN3 (Powertrain CAN)...  ");   Serial.println("");
  Serial.println("");

  DS1_ECU_LoggingConfigActive = 10001;

#pragma region // Setup - Pin Modes for  CAN Transcievers, LEDs, etc

  pinMode(32, OUTPUT);    // VIO pin setting for CAN3 transciever on KS and BasicDAZA v1.3x **This also illuninates Blue LED incicating CAN powered on
  digitalWrite(32, HIGH); // VIO high for CAN3 transciever on KS and BasicDAZA v1.3x

  #ifdef BasicDAZA_v2x0
  // pinMode(49, OUTPUT);          // Digital Output which powers Red LED for indicting CAN transmission cycle (BasicDAZA v2.0 and v2.1) 
  // digitalWrite(49, HIGH); // 

  #endif



#pragma endregion

#pragma region // Setup - CAN Communications for PT-CAN via CAN3


  IgnitionAwake = 1;
  ShutdownTimer = 24000;

  delay(50);
  TFTCAN3.begin();
  TFTCAN3.setBaudRate(500000);

  TFTCAN3.setMB(MB0, RX, STD); // Set mailbox as receiving standard frames.
  TFTCAN3.setMB(MB1, RX, STD); // Set mailbox as receiving standard frames.
  TFTCAN3.setMB(MB2, RX, STD); // Set mailbox as receiving standard frames.TFTCAN3.setMB(MB8, TX, STD); // Set mailbox as receiving standard frames.
  TFTCAN3.setMB(MB3, RX, STD); // Set mailbox as receiving standard frames.TFTCAN3.setMB(MB8, TX, STD); // Set mailbox as receiving standard frames.
  TFTCAN3.setMB(MB4, RX, STD); // Set mailbox as receiving standard frames.TFTCAN3.setMB(MB8, TX, STD); // Set mailbox as receiving standard frames.
  TFTCAN3.setMB(MB5, RX, STD); // Set mailbox as receiving standard frames.TFTCAN3.setMB(MB8, TX, STD); // Set mailbox as receiving standard frames.
  TFTCAN3.setMB(MB6, RX, STD); // Set mailbox as receiving standard frames.
  TFTCAN3.setMB(MB7, RX, STD); // Set mailbox as receiving standard frames.
  TFTCAN3.setMB(MB8, RX, STD); // Set mailbox as receiving standard frames.
  TFTCAN3.setMB(MB9, RX, STD); // Set mailbox as receiving standard frames.
  TFTCAN3.setMB(MB10, RX, STD); // Set mailbox as receiving standard frames.
  TFTCAN3.setMB(MB11, RX, STD); // Set mailbox as receiving standard frames.
  TFTCAN3.setMB(MB12, RX, STD); // Set mailbox as receiving standard frames.
  TFTCAN3.setMB(MB13, RX, STD); // Set mailbox as receiving standard frames.
  TFTCAN3.setMB(MB14, RX, STD); // Set mailbox as receiving standard frames.
  // TFTCAN3.setMB(MB15, RX, STD); // Set mailbox as receiving standard frames.


  // TFTCAN3.setMB(MB13, TX, STD); // Set mailbox as transmitting standard frames.
  // TFTCAN3.setMB(MB14, TX, STD); // Set mailbox as transmitting standard frames.
  // TFTCAN3.setMB(MB15, TX, STD); // Set mailbox as transmitting standard frames.
  TFTCAN3.setMB(MB16, TX, STD); // Set mailbox as transmitting standard frames.
  TFTCAN3.setMB(MB17, TX, STD); // Set mailbox as transmitting standard frames.
  TFTCAN3.setMB(MB18, TX, STD); // Set mailbox as transmitting standard frames.
  TFTCAN3.setMB(MB19, TX, STD); // Set mailbox as transmitting standard frames.
  TFTCAN3.setMB(MB20, TX, STD); // Set mailbox as transmitting standard frames.
  TFTCAN3.setMB(MB21, TX, STD); // Set mailbox as transmitting standard frames.
  // TFTCAN3.setMB(MB22, TX, STD); // Set mailbox as transmitting standard frames.
  // TFTCAN3.setMB(MB23, TX, STD); // Set mailbox as transmitting standard frames.
  // TFTCAN3.setMB(MB24, TX, STD); // Set mailbox as transmitting standard frames.


  TFTCAN3.setMBFilter(REJECT_ALL);
  TFTCAN3.enableMBInterrupts();

  TFTCAN3.onReceive(MB0, canSniff0x40); // mailbox and address for sniffing 0xXXX for Ign Status, to cease sending CAN messages
  TFTCAN3.setMBFilter(MB0, 0x40);

  TFTCAN3.onReceive(MB1, canSniff0x1AB); // Brake Pedal Switch Status
  TFTCAN3.setMBFilter(MB1, 0x1AB);

  TFTCAN3.onReceive(MB2, canSniff0x12b); // Cruise Control Status
  TFTCAN3.setMBFilter(MB2, 0x12b);

  TFTCAN3.onReceive(MB3, canSniff0x3DD); // Paddles Status
  TFTCAN3.setMBFilter(MB3, 0x3DD);

  TFTCAN3.onReceive(MB4, canSniffService22_PT_CAN_ECU_0x7E8); // PT_CAN to ECU Diagnostics ( for DTC Read/Clearing Resets via PT_CAN)
  TFTCAN3.setMBFilter(MB4, 0x7E8);
  //  TFTCAN3.setMBFilterRange(MB4, 0xFF, 0x7FF);

  TFTCAN3.onReceive(MB5, canSniffService22_PT_CAN_TCU_0x7E9_CatchAll); // PT_CAN to TCU Diagnostics ( for DQ500 Resets via PT_CAN)
  TFTCAN3.setMBFilter(MB5, 0x7E9);

  // TFTCAN3.onReceive(MB6, canSniff0x385); // MQB/Audi 8V Drive Select Modes
  // TFTCAN3.setMBFilter(MB6, 0x385);


  TFTCAN3.onReceive(MB7, canSniff0x391);
  TFTCAN3.setMBFilter(MB7, 0x391); // General SAE Engine Data (Accel Pedal, Load, EngCoolant, etc)
    
  TFTCAN3.onReceive(MB8, canSniff0x394);
  TFTCAN3.setMBFilter(MB8, 0x394); //  DQ500 Current Gear 

   #ifndef Concept_TFT_StandaloneHaldexController 
  TFTCAN3.onReceive(MB9, canSniff0x0A8);
  TFTCAN3.setMBFilter(MB9, 0xa8); // EngineRPM?
    #endif
  // TFTCAN3.onReceive(MB10, canSniff0x104);
  // TFTCAN3.setMBFilter(MB10, 0x104); // mailbox and address for sniffing Parking Brake Status

  #ifdef Concept_KitchenSink_SIC_Separate

  TFTCAN3.onReceive(MB6, canSniff0x783); // Frame1 from secondary TFT Module
  TFTCAN3.setMBFilter(MB6, 0x783);

  TFTCAN3.onReceive(MB10, canSniff0x788); // Frame2 from secondary TFT Module
  TFTCAN3.setMBFilter(MB10, 0x788);

  #endif


  TFTCAN3.onReceive(MB11, canSniff0xAD); // Gear Position Status
  TFTCAN3.setMBFilter(MB11, 0xAD);


  // TFTCAN3.onReceive(MB13, canSniff0x366); // Hazard Lights
  // TFTCAN3.setMBFilter(MB13, 0x366);

  #ifndef Concept_TFT_StandaloneHaldexController

  TFTCAN3.onReceive(MB13, canSniff0x789); // Frame from Syvecs AWD Controller
  TFTCAN3.setMBFilter(MB13, 0x789);

   TFTCAN3.onReceive(MB12, canSniff0xB2); // Wheel Speed Sensors
  TFTCAN3.setMBFilter(MB12, 0xB2);
  
 
  #endif

  // TFTCAN3.onReceive(MB13, canSniff0x790); // Frame from TFT Slave
  // TFTCAN3.setMBFilter(MB13, 0x790);

  // TFTCAN3.onReceive(MB2, canSniff0x107);
  // TFTCAN3.setMBFilter(MB2, 0x107); // Instrument Cluster's Tachometer value sent from DME (not directly linked to engine RPM)

  // TFTCAN3.onReceive(MB10, canSniff0x147); // Power/Torque Gauge Values
  // TFTCAN3.setMBFilter(MB10, 0x147);

  // TFTCAN3.onReceive(MB11, canSniff0x7EE); // DS1-Created frame(s) for RealDash
  // TFTCAN3.setMBFilter(MB11, 0x7EE);

#ifdef Concept_TFT_StandaloneHaldexController


  TFTCAN3.onReceive(MB10, canSniff0x792); //
  TFTCAN3.setMBFilter(MB10, 0x792);

  TFTCAN3.onReceive(MB10, canSniff0x793); //
  TFTCAN3.setMBFilter(MB10, 0x793);

  TFTCAN3.onReceive(MB8, canSniff0x86); // OEM MQB Steering Wheel Angle 
  TFTCAN3.setMBFilter(MB8, 0x86);

  TFTCAN3.onReceive(MB12, canSniff0xB2); // Wheel Speed Sensors
  TFTCAN3.setMBFilter(MB12, 0xB2);


#endif


  TFTCAN3.mailboxStatus();

  // delay(500);
   TFTCAN3.events(); 


#pragma endregion


#pragma region  // Setup - Swtiches for CAN Requests

  Switch_Req_PT_CAN_09_VIN = Switch_Req_PT_CAN_09_VIN_def;
  Switch_Req_PT_CAN_09_CALID = Switch_Req_PT_CAN_09_CALID_def;
  Switch_Req_PT_CAN_09_ECUName = Switch_Req_PT_CAN_09_ECUName_def;


  Switch_Req_22_PT_CAN_TCU_AllTqsAndPressures = Switch_Req_22_PT_CAN_TCU_AllTqsAndPressures_def;
  Switch_Req_22_PT_CAN_TCU_Set1_ClutchPressureActuals = Switch_Req_22_PT_CAN_TCU_Set1_ClutchPressureActuals_def;
  Switch_Req_22_PT_CAN_TCU_Set2_ClutchPressureTargets = Switch_Req_22_PT_CAN_TCU_Set2_ClutchPressureTargets_def;
  Switch_Req_22_PT_CAN_TCU_Set3_ClutchPlateTempActuals = Switch_Req_22_PT_CAN_TCU_Set3_ClutchPlateTempActuals_def;
  Switch_Req_22_PT_CAN_TCU_Set4_ClutchTorqueActuals = Switch_Req_22_PT_CAN_TCU_Set4_ClutchTorqueActuals_def;
  Switch_Req_22_PT_CAN_TCU_Set5_ClutchTorqueTargets = Switch_Req_22_PT_CAN_TCU_Set5_ClutchTorqueTargets_def;

PT_CAN_DQ500_DAQ_OLD_Set1_SequenceComplete = true;
  
#pragma endregion


Serial.println("");
Serial.print("Completed TFTCAN3 (Powertrain CAN) Initialization...  ");   Serial.println("");
Serial.println("");
Serial.println("");
Serial.println("//-------------------------------------------------------------------------------------------------------------------");


}

void TFTCAN_PT_CAN_AIM_init(){

  Serial.println("");
  Serial.print("Initializing TFTCAN3 (Powertrain CAN for AIM)...  ");   Serial.println("");
  Serial.println("");

  DS1_ECU_LoggingConfigActive = 10001;

#pragma region // Setup - Pin Modes for  CAN Transcievers, LEDs, etc

  pinMode(32, OUTPUT);    // VIO pin setting for CAN3 transciever on KS and BasicDAZA v1.3x **This also illuninates Blue LED incicating CAN powered on
  digitalWrite(32, HIGH); // VIO high for CAN3 transciever on KS and BasicDAZA v1.3x

  #ifdef BasicDAZA_v2x0
  // pinMode(49, OUTPUT);          // Digital Output which powers Red LED for indicting CAN transmission cycle (BasicDAZA v2.0 and v2.1) 
  // digitalWrite(49, HIGH); // 

  #endif



#pragma endregion

#pragma region // Setup - CAN Communications for PT-CAN via CAN3


  // IgnitionAwake = 1;
  // ShutdownTimer = 24000;

  delay(50);
  TFTCAN3.begin();
  TFTCAN3.setBaudRate(500000);

  TFTCAN3.setMB(MB10, RX, STD); // Set mailbox as receiving standard frames.
  TFTCAN3.setMB(MB11, RX, STD); // Set mailbox as receiving standard frames.
  TFTCAN3.setMB(MB12, RX, STD); // Set mailbox as receiving standard frames.TFTCAN3.setMB(MB8, TX, STD); // Set mailbox as receiving standard frames.
  // TFTCAN3.setMB(MB3, RX, STD); // Set mailbox as receiving standard frames.TFTCAN3.setMB(MB8, TX, STD); // Set mailbox as receiving standard frames.
  // TFTCAN3.setMB(MB4, RX, STD); // Set mailbox as receiving standard frames.TFTCAN3.setMB(MB8, TX, STD); // Set mailbox as receiving standard frames.
  // TFTCAN3.setMB(MB5, RX, STD); // Set mailbox as receiving standard frames.TFTCAN3.setMB(MB8, TX, STD); // Set mailbox as receiving standard frames.
  // TFTCAN3.setMB(MB6, RX, STD); // Set mailbox as receiving standard frames.
  // TFTCAN3.setMB(MB7, RX, STD); // Set mailbox as receiving standard frames.
  // TFTCAN3.setMB(MB8, RX, STD); // Set mailbox as receiving standard frames.
  // TFTCAN3.setMB(MB9, RX, STD); // Set mailbox as receiving standard frames.
  // TFTCAN3.setMB(MB10, RX, STD); // Set mailbox as receiving standard frames.
  // TFTCAN3.setMB(MB11, RX, STD); // Set mailbox as receiving standard frames.
  // TFTCAN3.setMB(MB12, RX, STD); // Set mailbox as receiving standard frames.
  // TFTCAN3.setMB(MB13, RX, STD); // Set mailbox as receiving standard frames.
  // TFTCAN3.setMB(MB14, RX, STD); // Set mailbox as receiving standard frames.
  // TFTCAN3.setMB(MB15, RX, STD); // Set mailbox as receiving standard frames.


  // TFTCAN3.setMB(MB13, TX, STD); // Set mailbox as transmitting standard frames.
  // TFTCAN3.setMB(MB14, TX, STD); // Set mailbox as transmitting standard frames.
  // TFTCAN3.setMB(MB15, TX, STD); // Set mailbox as transmitting standard frames.
  TFTCAN3.setMB(MB16, TX, STD); // Set mailbox as transmitting standard frames.
  // TFTCAN3.setMB(MB17, TX, STD); // Set mailbox as transmitting standard frames.
  // TFTCAN3.setMB(MB18, TX, STD); // Set mailbox as transmitting standard frames.
  // TFTCAN3.setMB(MB19, TX, STD); // Set mailbox as transmitting standard frames.
  // TFTCAN3.setMB(MB20, TX, STD); // Set mailbox as transmitting standard frames.
  // TFTCAN3.setMB(MB21, TX, STD); // Set mailbox as transmitting standard frames.
  // TFTCAN3.setMB(MB22, TX, STD); // Set mailbox as transmitting standard frames.
  // TFTCAN3.setMB(MB23, TX, STD); // Set mailbox as transmitting standard frames.
  // TFTCAN3.setMB(MB24, TX, STD); // Set mailbox as transmitting standard frames.


  TFTCAN3.setMBFilter(REJECT_ALL);
  TFTCAN3.enableMBInterrupts();

  // TFTCAN3.onReceive(MB0, canSniff0x40); // mailbox and address for sniffing 0xXXX for Ign Status, to cease sending CAN messages
  // TFTCAN3.setMBFilter(MB0, 0x40);

  // TFTCAN3.onReceive(MB1, canSniff0x1AB); // Brake Pedal Switch Status
  // TFTCAN3.setMBFilter(MB1, 0x1AB);

  // TFTCAN3.onReceive(MB2, canSniff0x12b); // Cruise Control Status
  // TFTCAN3.setMBFilter(MB2, 0x12b);

  // TFTCAN3.onReceive(MB3, canSniff0x3DD); // Paddles Status
  // TFTCAN3.setMBFilter(MB3, 0x3DD);

  // TFTCAN3.onReceive(MB4, canSniffService22_PT_CAN_ECU_0x7E8); // PT_CAN to ECU Diagnostics ( for DTC Read/Clearing Resets via PT_CAN)
  // TFTCAN3.setMBFilter(MB4, 0x7E8);
  //  TFTCAN3.setMBFilterRange(MB4, 0xFF, 0x7FF);

  // TFTCAN3.onReceive(MB5, canSniffService22_PT_CAN_TCU_0x7E9_CatchAll); // PT_CAN to TCU Diagnostics ( for DQ500 Resets via PT_CAN)
  // TFTCAN3.setMBFilter(MB5, 0x7E9);

  // TFTCAN3.onReceive(MB6, canSniff0x385); // MQB/Audi 8V Drive Select Modes
  // TFTCAN3.setMBFilter(MB6, 0x385);


  TFTCAN3.onReceive(MB7, canSniff0x391);
  TFTCAN3.setMBFilter(MB7, 0x391); // General SAE Engine Data (Accel Pedal, Load, EngCoolant, etc)
    
  // TFTCAN3.onReceive(MB8, canSniff0x394);
  // TFTCAN3.setMBFilter(MB8, 0x394); //  DQ500 Current Gear 

   #ifndef Concept_TFT_StandaloneHaldexController 
  TFTCAN3.onReceive(MB9, canSniff0x0A8);
  TFTCAN3.setMBFilter(MB9, 0xa8); // EngineRPM?
    #endif
  // TFTCAN3.onReceive(MB10, canSniff0x104);
  // TFTCAN3.setMBFilter(MB10, 0x104); // mailbox and address for sniffing Parking Brake Status

  #ifdef Concept_KitchenSink_SIC_Separate

  TFTCAN3.onReceive(MB6, canSniff0x783); // Frame1 from secondary TFT Module
  TFTCAN3.setMBFilter(MB6, 0x783);

  TFTCAN3.onReceive(MB10, canSniff0x788); // Frame2 from secondary TFT Module
  TFTCAN3.setMBFilter(MB10, 0x788);

  #endif


  // TFTCAN3.onReceive(MB11, canSniff0xAD); // Gear Position Status
  // TFTCAN3.setMBFilter(MB11, 0xAD);


  // TFTCAN3.onReceive(MB13, canSniff0x366); // Hazard Lights
  // TFTCAN3.setMBFilter(MB13, 0x366);

  #ifndef Concept_TFT_StandaloneHaldexController

  TFTCAN3.onReceive(MB13, canSniff0x789); // Frame from Syvecs AWD Controller
  TFTCAN3.setMBFilter(MB13, 0x789);

   TFTCAN3.onReceive(MB12, canSniff0xB2); // Wheel Speed Sensors
  TFTCAN3.setMBFilter(MB12, 0xB2);
  
 
  #endif

  // TFTCAN3.onReceive(MB13, canSniff0x790); // Frame from TFT Slave
  // TFTCAN3.setMBFilter(MB13, 0x790);

  // TFTCAN3.onReceive(MB2, canSniff0x107);
  // TFTCAN3.setMBFilter(MB2, 0x107); // Instrument Cluster's Tachometer value sent from DME (not directly linked to engine RPM)

  // TFTCAN3.onReceive(MB10, canSniff0x147); // Power/Torque Gauge Values
  // TFTCAN3.setMBFilter(MB10, 0x147);

  // TFTCAN3.onReceive(MB11, canSniff0x7EE); // DS1-Created frame(s) for RealDash
  // TFTCAN3.setMBFilter(MB11, 0x7EE);

#ifdef Concept_TFT_StandaloneHaldexController


  // TFTCAN3.onReceive(MB10, canSniff0x792); //
  // TFTCAN3.setMBFilter(MB10, 0x792);

  // TFTCAN3.onReceive(MB10, canSniff0x793); //
  // TFTCAN3.setMBFilter(MB10, 0x793);

  // TFTCAN3.onReceive(MB8, canSniff0x86); // OEM MQB Steering Wheel Angle 
  // TFTCAN3.setMBFilter(MB8, 0x86);

  // TFTCAN3.onReceive(MB12, canSniff0xB2); // Wheel Speed Sensors
  // TFTCAN3.setMBFilter(MB12, 0xB2);


#endif


  TFTCAN3.mailboxStatus();

  // delay(500);
   TFTCAN3.events(); 


#pragma endregion




Serial.println("");
Serial.print("Completed TFTCAN3 (Powertrain CAN for AIM) Initialization...  ");   Serial.println("");
Serial.println("");
Serial.println("");
Serial.println("//-------------------------------------------------------------------------------------------------------------------");


}

void do_TFTCAN_PT_CAN_Sniffing(){
  
 TFTCAN3.events(); 

  // digitalToggle(2);

  // GaugeTimerInt1 = GaugeTimer1;
  // // GaugeTimer1 = GaugeTimerInt1;

  // GaugeTimerInt2 = GaugeTimer2;
  // // GaugeTimer2 = GaugeTimerInt2;

  // GaugeTimerInt3 = GaugeTimer3;
  // // GaugeTimer3 = GaugeTimerInt3;


  #pragma region // CAN Input processing



    #pragma region      // Process 0x40 first byte - Determine Ignition Status

    //  IgnitionStatus = IgnitionStatusCAN;

  //   if (IgnitionStatus == 65504)
  //   {
  //     IgnitionAwake = 0;
  //     ShutdownTimer = 0;
      
  //   }

  //   else if (IgnitionStatus != 65504 || IgnitionStatus == 0)
  //   {
  //      IgnitionAwake = 1;
  //      ShutdownTimer = 25000;
  //   }

  // if (ShutdownTimer < 20000) {
  //   IgnitionAwake = 0;
  // }


  if(IgnitionStatusTimer_TimeSinceLastMessage < 1000)
  {
    IgnitionAwake = 1;
  }
  else {
    IgnitionAwake = 0;
  }

  #pragma endregion

    #pragma region      // Process 0x0A8 - Engine RPM

  if(EngineRPM_0xA8 > 600 && EngineRPM_0xA8 < 1500)  //  indicates engine RPM is within the window for DQ500 Resets
  {
  EngineRPM_IdleStatus = 1;
  // Serial.println("DQ500Reset_EngRPMAcceptableCondition ");
  }
    else
    {
  EngineRPM_IdleStatus = 0;
    }


  if(EngineRPM_0xA8 > 400)  //  indicates engine RPM is at 0 for IMU resets or dyno mode activation
  {
  EngineRPM_ON = 1;
  EngineRPM_OFF = 0;

  }
    else
    {
      EngineRPM_ON = 0;
      EngineRPM_OFF = 1;

    }


    #pragma endregion

    #pragma region      // Process 0xAD (byte 5 only) - Gear Lever Position via PT-CAN

  // 0x14 = Park
  // 0x18 = Reverse
  // 0x1C = Neutral
  // 0x20 = Drive
  // 0x34 or 0x38 = TIP

    if(GearLeverPosition_0xAD == 0x14)  //0x14 indicates gear lever is in Park position
    {
        GearLeverPosition_Park = 1;
        GearLeverPosition_Reverse = 0;
        GearLeverPosition_Neutral = 0;
        GearLeverPosition_Drive= 0;
        // GearLeverPosition_TIP = 0; // Now using status from 0x394
    }
    if(GearLeverPosition_0xAD == 0x18)  //0x18 indicates gear lever is in Reverse position
    {
        GearLeverPosition_Park = 0;
        GearLeverPosition_Reverse = 1;
        GearLeverPosition_Neutral = 0;
        GearLeverPosition_Drive= 0;
        // GearLeverPosition_TIP = 0; // Now using status from 0x394
    }
    if(GearLeverPosition_0xAD == 0x1C)  //0x1C indicates gear lever is in Neutral position
    {
        GearLeverPosition_Park = 0;
        GearLeverPosition_Reverse = 0;
        GearLeverPosition_Neutral = 1;
        GearLeverPosition_Drive= 0;
        // GearLeverPosition_TIP = 0;  // Now using status from 0x394
    }
    if(GearLeverPosition_0xAD == 0x20 || GearLeverPosition_0xAD == 0x24)  //0x20 or 0x24 indicates gear lever is in Drive position
    {
        GearLeverPosition_Park = 0;
        GearLeverPosition_Reverse = 0;
        GearLeverPosition_Neutral = 0;
        GearLeverPosition_Drive= 1;
        // GearLeverPosition_TIP = 0; // Now using status from 0x394
    }
    if(GearLeverPosition_0xAD == 0x34 || GearLeverPosition_0xAD == 0x38)  //0x34 or 0x36 indicates gear lever is in TIP position
    {
        GearLeverPosition_Park = 0;
        GearLeverPosition_Reverse = 0;
        GearLeverPosition_Neutral = 0;
        GearLeverPosition_Drive= 0;
        // GearLeverPosition_TIP = 1; // Now using status from 0x394
    }



      unsigned char myByteGoalGear0xAD = PT_CAN_msg0xAD_buf7_Raw; // Example byte
      int startBit0xAD = 4; // Starting position of the range (0-indexed)
      int numBits0xAD = 4;  // Number of bits to extract

      // Create a mask with 'numBits' set to 1
      unsigned char mask0xAD = (1 << numBits0xAD) - 1; 

      // Shift the desired range to the least significant position and then apply the mask
      unsigned char extractedBitsGoalGear0xAD = (myByteGoalGear0xAD >> startBit0xAD) & mask0xAD;

      DQ500_GoalGear0xAD_CAN = extractedBitsGoalGear0xAD;
      DQ500_GoalGear0xAD = DQ500_GoalGear0xAD_CAN;

      DQ500_ClutchLockStatus = PT_CAN_msg0xAD_buf6_Raw;



    #pragma endregion

    #pragma region      // Process 0xB2 - Individual Wheel Speeds


      #pragma region // Calculate Individual Wheel Speeds in km/h based on VAG CAN sniffing

        word WhlSpdFL_Raw_CAN = (PT_CAN_msg0xB2x1buf4 + (PT_CAN_msg0xB2x1buf5 * 256));
        WhlSpdFL_Raw = WhlSpdFL_Raw_CAN;
        WhlSpdFL = (WhlSpdFL_Raw * 0.0075);
        WhlSpdFL_Float = (WhlSpdFL_Raw * 0.0075);

        word WhlSpdFR_Raw_CAN = (PT_CAN_msg0xB2x1buf6 + (PT_CAN_msg0xB2x1buf7 * 256));
        WhlSpdFR_Raw = WhlSpdFR_Raw_CAN;
        WhlSpdFR = (WhlSpdFR_Raw* 0.0075);
        WhlSpdFR_Float = (WhlSpdFR_Raw* 0.0075);

        word WhlSpdRL_Raw_CAN = (PT_CAN_msg0xB2x1buf0 + (PT_CAN_msg0xB2x1buf1 * 256));
        WhlSpdRL_Raw = WhlSpdRL_Raw_CAN;
        WhlSpdRL = (WhlSpdRL_Raw * 0.0075);
        WhlSpdRL_Float = (WhlSpdRL_Raw * 0.0075);

        word WhlSpdRR_Raw_CAN = (PT_CAN_msg0xB2x1buf2 + (PT_CAN_msg0xB2x1buf3 * 256));
        WhlSpdRR_Raw = WhlSpdRR_Raw_CAN;
        WhlSpdRR = (WhlSpdRR_Raw * 0.0075);
        WhlSpdRR_Float = (WhlSpdRR_Raw * 0.0075);

        // word WhlSpdFL_Raw_CAN = (PT_CAN_msg0xB2.buf[0] + (PT_CAN_msg0xB2.buf[1] * 256));
        // WhlSpdFL_Raw = WhlSpdFL_Raw_CAN;
        // WhlSpdFL = (WhlSpdFL_Raw * 0.0075);

        // word WhlSpdFR_Raw_CAN = (PT_CAN_msg0xB2.buf[2] + (PT_CAN_msg0xB2.buf[3] * 256));
        // WhlSpdFR_Raw = WhlSpdFR_Raw_CAN;
        // WhlSpdFR = (WhlSpdFR_Raw* 0.0075);

        // word WhlSpdRL_Raw_CAN = (PT_CAN_msg0xB2.buf[4] + (PT_CAN_msg0xB2.buf[5] * 256));
        // WhlSpdRL_Raw = WhlSpdRL_Raw_CAN;
        // WhlSpdRL = (WhlSpdRL_Raw * 0.0075);

        // word WhlSpdRR_Raw_CAN = (PT_CAN_msg0xB2.buf[6] + (PT_CAN_msg0xB2.buf[7] * 256));
        // WhlSpdRR_Raw = WhlSpdRR_Raw_CAN;
        // WhlSpdRR = (WhlSpdRR_Raw * 0.0075);


      //  Serial.print("WhlSpdFL: "); Serial.print(WhlSpdFL); Serial.println();
      //  Serial.print("WhlSpdFR: "); Serial.print(WhlSpdFR); Serial.println();
      //  Serial.print("WhlSpdRL: "); Serial.print(WhlSpdRL); Serial.println();
      //  Serial.print("WhlSpdRR: "); Serial.print(WhlSpdRR); Serial.println();


        #pragma endregion

      #pragma region // Calculate Axle Averages and Slip Rates

          WhlSpdAvg_Float = (WhlSpdFL_Float + WhlSpdFR_Float + WhlSpdRL_Float + WhlSpdRR_Float) / 4.0;
          WhlSpd_FrontAxleAvg_Float = (WhlSpdFL_Float + WhlSpdFR_Float) / 2.0;
          WhlSpd_RearAxleAvg_Float =  (WhlSpdRL_Float + WhlSpdRR_Float) / 2.0;

          if(WhlSpdAvg_Float == 0.0) {
            TimeSinceWhlSpd0 = 0;
          }

      if(WhlSpdFL_Float > 0) {
          // Front Axle Slip Multiplier
          if(WhlSpdFL_Float > WhlSpdFR_Float) {
          WhlSpd_FrontAxleSlipKmh_Float = WhlSpdFL_Float - WhlSpdFR_Float;
          WhlSpd_FrontAxleSlipPct_Float = WhlSpdFL_Float / WhlSpdFR_Float;
          // (WhlSpd_FrontAxleSlipPct_Float = WhlSpdFL_Float / WhlSpdFR_Float) - 1.0;
          }
            else if(WhlSpdFL_Float > WhlSpdFR_Float) {
            WhlSpd_FrontAxleSlipKmh_Float = WhlSpdFR_Float - WhlSpdFL_Float;
            WhlSpd_FrontAxleSlipPct_Float = WhlSpdFR_Float / WhlSpdFL_Float;
          //  (WhlSpd_FrontAxleSlipPct_Float = WhlSpdFR_Float / WhlSpdFL_Float) - 1.0;
            }

          // Rear Axle Slip Multiplier
          if(WhlSpdRL_Float > WhlSpdRR_Float) {
          WhlSpd_RearAxleSlipKmh_Float = WhlSpdRL_Float - WhlSpdRR_Float;
          WhlSpd_RearAxleSlipPct_Float = WhlSpdRL_Float / WhlSpdRR_Float;
          // (WhlSpd_RearAxleSlipPct_Float = WhlSpdRL_Float / WhlSpdRR_Float) - 1.0;
          }
            else if(WhlSpdRL_Float > WhlSpdRR_Float) {
            WhlSpd_RearAxleSlipKmh_Float = WhlSpdRR_Float - WhlSpdRL_Float;
            WhlSpd_RearAxleSlipPct_Float = WhlSpdRR_Float / WhlSpdRL_Float;
            // (WhlSpd_RearAxleSlipPct_Float = WhlSpdRR_Float / WhlSpdRL_Float) - 1.0;
            }

          // Center or Front/Rear Slip Multiplier
          if(WhlSpd_FrontAxleAvg_Float > WhlSpd_RearAxleAvg_Float) {
          WhlSpd_CenterSlipKmh_Float = WhlSpd_FrontAxleAvg_Float - WhlSpd_RearAxleAvg_Float;
          WhlSpd_CenterSlipPct_Float = WhlSpd_FrontAxleAvg_Float / WhlSpd_RearAxleAvg_Float;
          // (WhlSpd_CenterSlipPct_Float = WhlSpd_FrontAxleAvg_Float / WhlSpd_RearAxleAvg_Float) - 1.0;
          }
            else if(WhlSpd_FrontAxleAvg_Float > WhlSpd_RearAxleAvg_Float) {
            WhlSpd_CenterSlipKmh_Float = WhlSpd_RearAxleAvg_Float - WhlSpd_FrontAxleAvg_Float;
            WhlSpd_CenterSlipPct_Float = WhlSpd_RearAxleAvg_Float / WhlSpd_FrontAxleAvg_Float;
            // (WhlSpd_CenterSlipPct_Float = WhlSpd_RearAxleAvg_Float / WhlSpd_FrontAxleAvg_Float) - 1.0;
            }

          }

          else {

          WhlSpd_FrontAxleSlipKmh_Float = 1.0 ;
          WhlSpd_FrontAxleSlipPct_Float = 1.0 ;

          WhlSpd_RearAxleSlipKmh_Float = 1.0 ;
          WhlSpd_RearAxleSlipPct_Float = 1.0 ;

          WhlSpd_CenterSlipKmh_Float = 1.0 ;
          WhlSpd_CenterSlipPct_Float = 1.0 ;


          }
          #pragma endregion


    #pragma endregion

    #pragma region      // Process 0x104 - ParkBrake Status

  if(ParkBrakeStatus_0x104 == 0x31 || ParkBrakeStatus_0x104 == 0x32)  //0x31 indicates the Park Brake position is activated
  {
  ParkBrakeStatus = 1;
  }
    else
    {
  ParkBrakeStatus = 0;
    }



    #pragma endregion

    #pragma region      // Process 0x106 - Brake Pressure - OEM MQB

        //starts at bit 16, 10bit length, unsigned,  (0.3, -30) [-30 | 276.6] "Unit Bar" 

        // BrakePressureBAR_0x106_CAN = (PT_CAN_msg0x106_buf0_Raw + (PT_CAN_msg0x106_buf1_Raw * 256));
        // BrakePressureBAR_0x106_Raw = BrakePressureBAR_0x106_CAN;

        // BrakePressureBAR_Float = BrakePressureBAR_0x106_Raw * 0.01;


        unsigned char myByteBrakePressureBAR_0x106 = (PT_CAN_msg0x394_buf2_Raw + PT_CAN_msg0x394_buf3_Raw); // Example byte
        int startBit0x106 = 16; // Starting position of the range (0-indexed)
        int numBits0x106 = 10;  // Number of bits to extract

        // Create a mask with 'numBits' set to 1
        unsigned char mask0x106 = (1 << numBits0x106) - 1; 

        // Shift the desired range to the least significant position and then apply the mask
        unsigned char extractedBits_BrakePressureBAR_0x106 = (myByteBrakePressureBAR_0x106 >> startBit0x106) & mask0x106;

        BrakePressureBAR_0x106_Extracted = extractedBits_BrakePressureBAR_0x106;
        BrakePressureBAR_0x106_Raw = BrakePressureBAR_0x106_Extracted;


    #pragma region      // Process 0x394 - DQ500 Target Gear and Current Gear (and TIP Lever Position





    #pragma endregion




    #pragma endregion

    #pragma region      // Process 0x86 - SteeringWheelAngle - OEM MQB // commented out as it is now done via SuspCAN



      //   //starts at bit 16, 10bit length, unsigned,  (0.3, -30) [-30 | 276.6] "Unit Bar" 


      //   MQB_SteeringWheelAngle_0x86_CAN = (PT_CAN_msg0x86_buf2_Raw + (PT_CAN_msg0x86_buf3_Raw * 256) );
      //   MQB_SteeringWheelAngle_0x86_Raw = MQB_SteeringWheelAngle_0x86_CAN;


      //   if(MQB_SteeringWheelAngle_0x86_CAN < 100) { // steering wheel turned counter-clockwise
      //   MQB_SteeringWheelAngle_0x86_Float = map(MQB_SteeringWheelAngle_0x86_CAN,0,28,0,180);
      //   }

      // unsigned char myByteSWA_0x86x2 = (PT_CAN_msg0x86_buf2_Raw ); // Example byte
      // int startBit0x86x2 = 0; // Starting position of the range (0-indexed)
      // int numBits0x86x2 = 12;  // Number of bits to extract
      // // Create a mask with 'numBits' set to 1
      // unsigned char mask0x86x2 = (1 << numBits0x86x2) - 1; 
      // // Shift the desired range to the least significant position and then apply the mask
      // unsigned char extractedBits_SWA_0x86x2 = (myByteSWA_0x86x2 >> startBit0x86x2) & mask0x86x2;
      // unsigned char MQB_SteeringWheelAngle_0x86x2_Extracted = extractedBits_SWA_0x86x2;





      // unsigned char myByteSWA_0x86x3 = (PT_CAN_msg0x86_buf3_Raw ); // Example byte
      // int startBit0x86x3 = 0; // Starting position of the range (0-indexed)
      // int numBits0x86x3 = 4;  // Number of bits to extract
      // // Create a mask with 'numBits' set to 1
      // unsigned char mask0x86x3 = (1 << numBits0x86x3) - 1; 

      // // Shift the desired range to the least significant position and then apply the mask
      // unsigned char extractedBits_SWA_0x86x3 = (myByteSWA_0x86x3 >> startBit0x86x3) & mask0x86x3;

      // unsigned char MQB_SteeringWheelAngle_0x86x3_Extracted = extractedBits_SWA_0x86x3;


      // // MQB_SteeringWheelAngle_0x86_Raw = MQB_SteeringWheelAngle_0x86x3_Extracted;
      // MQB_SteeringWheelAngle_0x86_Raw = (MQB_SteeringWheelAngle_0x86x2_Extracted + (MQB_SteeringWheelAngle_0x86x3_Extracted * 256) );
      // MQB_SteeringWheelAngle_0x86_Int = MQB_SteeringWheelAngle_0x86_Raw;


    #pragma endregion

    #pragma region      // Process 0x107 - Tachometer Engine RPM via PT-CAN, from DME to Instrument Cluster


    MQB_TachEngineRPM_Sniffed_0x107_Raw = (PT_CAN_msg0x107_buf3_Raw + (PT_CAN_msg0x107_buf4_Raw *256));
    MQB_TachEngineRPM_Sniffed_0x107_Int = MQB_TachEngineRPM_Sniffed_0x107_Raw / 3;


    #pragma endregion

    #pragma region      // Process 0x120 byte 3 - Cruise Stalk Static Position

      if(cruiseStalkStaticPosition_0x120 == 2) { // 0x06/ Dec 6 = cruise Stalk in Static ON position, no button pressed
          CruiseStalk_ON = 1;
          CruiseStalk_OFF = 0;
          }

      if(cruiseStalkStaticPosition_0x120 == 00) { // 0x00 =cruise Stalk in Static OFF position
          CruiseStalk_ON = 0;
          CruiseStalk_OFF = 1;

          }

    #pragma endregion

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

    #pragma region      // Process 0x1AB - Brake Pedal Switch

      if(BrakeSwitch_0x1AB == 1 || BrakeSwitch_0x1AB == 5)  // 0x1 or 0x5 indicates the brake pedal is being pressed (maybe 0xE as well?)
      {
      BrakePedalSwitchStatus = 1;
      }
        else
        {
        BrakePedalSwitchStatus = 0;
        }


    #pragma endregion

    #pragma region      // Process 0x366 - Hazard Lights Button

      if(HazardLightsButton_0x366 == 0x10)  // byte2 value of 0x00 indicates the Hazard Lights are OFF, 0x10 is ON
      {
      HazardLightsButton = 1;
      }
        else
        {
        HazardLightsButton = 0;
        }


    #pragma endregion

    #pragma region      // Process 0x385 -  Drive Select Modes

      #pragma region  // Individual ACC Modes

          if ( DriveSelect_ACC_Mode_Int == 33 || DriveSelect_ACC_Mode_Int == 35 ) // This indicates that the ACC Individual Setting is set to Auto
            {
              DriveSelect_ACC_Mode_Auto = 1;
              DriveSelect_ACC_Mode_Comfort = 0;
              DriveSelect_ACC_Mode_Dynamic = 0;
            }

          if ( DriveSelect_ACC_Mode_Int == 17 || DriveSelect_ACC_Mode_Int == 19 ) // This indicates that the ACC Individual Setting is set to Comfort
            {
              DriveSelect_ACC_Mode_Auto = 0;
              DriveSelect_ACC_Mode_Comfort = 1;
              DriveSelect_ACC_Mode_Dynamic = 0;
            }

          if ( DriveSelect_ACC_Mode_Int == 49 || DriveSelect_ACC_Mode_Int == 51 ) // This indicates that the ACC Individual Setting is set to Dynamic
            {
              DriveSelect_ACC_Mode_Auto = 0;
              DriveSelect_ACC_Mode_Comfort = 0;
              DriveSelect_ACC_Mode_Dynamic = 1;
            }



      #pragma endregion


    #pragma endregion

    #pragma region      // Process 0x391 - Accel Pedal via PT-CAN from DME 


      // ThrottleTPS_0x391_CAN = (PT_CAN_msg0x391.buf[2]) ;
      // ThrottleTPS_0x391 = (ThrottleTPS_0x391_CAN / 0.39215686275 ) ;

    #pragma endregion

    #pragma region      // Process 0x394 - DQ500 Target Gear and Current Gear (and TIP Lever Position



      unsigned char myByteTgtGear2 = PT_CAN_msg0x394_buf1_Raw; // Example byte
      int startBit0x394 = 4; // Starting position of the range (0-indexed)
      int numBits0x394 = 4;  // Number of bits to extract

      // Create a mask with 'numBits' set to 1
      unsigned char mask0x394 = (1 << numBits0x394) - 1; 

      // Shift the desired range to the least significant position and then apply the mask
      unsigned char extractedBitsTgtGear2 = (myByteTgtGear2 >> startBit0x394) & mask0x394;

      DQ500_TargetGear2_0x394_CAN = extractedBitsTgtGear2;
      DQ500_TargetGear2_0x394 = DQ500_TargetGear2_0x394_CAN;





      DQ500_TargetGear_0x394_CAN = PT_CAN_msg0x394_buf2_Raw;
      DQ500_TargetGear_0x394 = DQ500_TargetGear_0x394_CAN;

      DQ500_CurrentGear_0x394_CAN = PT_CAN_msg0x394_buf3_Raw;
      DQ500_CurrentGear_0x394 = DQ500_CurrentGear_0x394_CAN;



      if(PT_CAN_msg0x394_buf5_Raw > 0x10) {
            GearLeverPosition_TIP = 1;
        }
      else {
            GearLeverPosition_TIP = 0;
        }



    #pragma endregion

    #pragma region      // Process 0x3DD byte 7 - Paddle Positions 

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


    #pragma endregion

    #pragma region      // Process 0x7EE - DS1 Custom Multiplexed Frame - Current MapSlot, Gear, PedalPosition, Tq Request(?), etc

      // if(PaddlePositions == 0x00)
      //   {
      //   Paddle_DOWN_Pulled = 0;
      //   Paddle_UP_Pulled = 0;
      //   BOTH_Paddles_Pulled = 0;
      //   }

      // if(PaddlePositions == 0x01)
      //   {
      //   Paddle_DOWN_Pulled = 1;
      //   Paddle_UP_Pulled = 0;
      //   BOTH_Paddles_Pulled = 0;
      //   }

      // if(PaddlePositions == 0x02)
      //   {
        
      //   Paddle_DOWN_Pulled = 0;
      //   Paddle_UP_Pulled = 1;
      //   BOTH_Paddles_Pulled = 0;
      //   }

      // if(PaddlePositions == 0x03)
      //   {
      //   Paddle_DOWN_Pulled = 0;
      //   Paddle_UP_Pulled = 0;
      //   BOTH_Paddles_Pulled = 1;
      //   }


    #pragma endregion

    #pragma region      // Process 0x743 - LS Outputs - All Bytes 


        LS1_Signalvia0x743_Int = (PT_CAN_msg0x743_buf0_Raw + (PT_CAN_msg0x743_buf1_Raw *256)) ;
        LS2_Signalvia0x743_Int = (PT_CAN_msg0x743_buf2_Raw + (PT_CAN_msg0x743_buf3_Raw *256)) ;
        LS3_Signalvia0x743_Int = (PT_CAN_msg0x743_buf4_Raw + (PT_CAN_msg0x743_buf5_Raw *256)) ;
        LS4_Signalvia0x743_Int = (PT_CAN_msg0x743_buf6_Raw + (PT_CAN_msg0x743_buf7_Raw *256)) ;


    #pragma endregion

    #pragma region      // Process 0x783 and 0x788 - SIC Data from SIC


      word EngineRPM_0x783_CAN = (PT_CAN_msg0x783_buf0_Raw + (PT_CAN_msg0x783_buf1_Raw * 256 ) );
      EngineRPM_0x783 = (EngineRPM_0x783_CAN / 4);

      word EngineRPM_0x788_CAN = (PT_CAN_msg0x788_buf0_Raw + (PT_CAN_msg0x788_buf1_Raw * 256 ) );
      EngineRPM_0x788 = (EngineRPM_0x788_CAN / 4);

      word EngineRPM_DOT_0x788_CAN = (PT_CAN_msg0x788_buf4_Raw + (PT_CAN_msg0x788_buf5_Raw * 256 ) );
      EngineRPM_DOT_0x788 = (EngineRPM_DOT_0x788_CAN / 4);
  // PT_CAN_msg0x788


    #pragma endregion


    #pragma region      // Process 0x789 and 0x790 - Syvecs AWD Data


      SyvecsAWD_AWDPot_CAN = ((PT_CAN_msg0x789_buf0_Raw + (PT_CAN_msg0x789_buf1_Raw * 256 )) );
      SyvecsAWD_AWDPotFloat = (((SyvecsAWD_AWDPot_CAN) * 1000) / 8192.0);
      // int SyvecsAWD_AWDPotInt = SyvecsAWD_AWDPotFloat;
      SyvecsAWD_AWDPot = SyvecsAWD_AWDPotFloat / 10.0;

      word SyvecsAWD_AWDBase_CAN = (PT_CAN_msg0x789_buf2_Raw + (PT_CAN_msg0x789_buf3_Raw * 256 ) );
      // SyvecsAWD_AWDBase = (SyvecsAWD_AWDBase_CAN);
      SyvecsAWD_AWDBaseFloat = (SyvecsAWD_AWDBase_CAN / 81.92);
      SyvecsAWD_AWDBase = (SyvecsAWD_AWDBaseFloat / 1.0);

      word SyvecsAWD_AWDFinal_CAN = (PT_CAN_msg0x789_buf4_Raw + (PT_CAN_msg0x789_buf5_Raw * 256 ) );
      // SyvecsAWD_AWDFinal = (SyvecsAWD_AWDFinal_CAN);
      SyvecsAWD_AWDFinalFloat = (SyvecsAWD_AWDFinal_CAN) / 81.92;
      SyvecsAWD_AWDFinal = (SyvecsAWD_AWDFinalFloat / 1.0);

      word SyvecsAWD_CenterSlipAdder_CAN = (PT_CAN_msg0x789_buf6_Raw + (PT_CAN_msg0x789_buf7_Raw * 256 ) );
      SyvecsAWD_CenterSlipAdder_Float = (SyvecsAWD_CenterSlipAdder_CAN / 81.92);
      SyvecsAWD_CenterSlipAdder = (SyvecsAWD_CenterSlipAdder_Float / 1.0);


      SyvecsAWD_AWDPotMultiplier_CAN = ((PT_CAN_msg0x790_buf0_Raw + (PT_CAN_msg0x790_buf1_Raw * 256 )) );
      SyvecsAWD_AWDPotMultiplier_Float = (((SyvecsAWD_AWDPotMultiplier_CAN) * 1000) / 8192.0);
      SyvecsAWD_AWDPotMultiplier_Int = SyvecsAWD_AWDPotMultiplier_Float / 1.0;

      SyvecsAWD_FrontSlipCalc_CAN = ((PT_CAN_msg0x790_buf2_Raw + (PT_CAN_msg0x790_buf3_Raw * 256 )) );
      SyvecsAWD_FrontSlipCalc_Float = (((SyvecsAWD_FrontSlipCalc_CAN) * 1000) / 8192.0);
      SyvecsAWD_FrontSlipCalc_Int= SyvecsAWD_FrontSlipCalc_Float / 1.0;

      SyvecsAWD_RearSlipCalc_CAN = ((PT_CAN_msg0x790_buf4_Raw + (PT_CAN_msg0x790_buf5_Raw * 256 )) );
      SyvecsAWD_RearSlipCalc_Float = (((SyvecsAWD_RearSlipCalc_CAN) * 1000) / 8192.0);
      SyvecsAWD_RearSlipCalc_Int= SyvecsAWD_RearSlipCalc_Float / 1.0;

      SyvecsAWD_CenterSlipCalc_CAN = ((PT_CAN_msg0x790_buf6_Raw + (PT_CAN_msg0x790_buf7_Raw * 256 )) );
      SyvecsAWD_CenterSlipCalc_Float = (((SyvecsAWD_CenterSlipCalc_CAN) * 1000) / 8192.0);
      SyvecsAWD_CenterSlipCalc_Int= SyvecsAWD_CenterSlipCalc_Float / 1.0;



    #pragma endregion







    #pragma endregion

#pragma endregion



}

void do_TFTCAN_PT_CAN_Transmits_OEM_ConfigurationModes_Resets(){


  #pragma region // Determine Conditions, Prepare, and Execute canTx PT_CAN Service22 Executable Functions - OEM Diagnostic Functions, DQ500 Resets, etc

    #pragma region // Send CAN Tx PT_CAN KeepAwake Message to 0x700 for Config Modes that require keepawakes

      if ( ConfigurationUpdateMode1_Active == 1 && loopDelayPT_CAN_KeepAwake_0x700 > 2000 ) {

        if (   MQB_ConfigurationMode_JobNumber_to_Update == Job_DTC_Clear
            || MQB_ConfigurationMode_JobNumber_to_Update == Job_LPFP_Activation
            || MQB_ConfigurationMode_JobNumber_to_Update == Job_DynoMode_Activation
            )
          {
            // canTx_PT_CAN_ECU_GeneralKeepAwake_0x700_2_3E_80_55();
            canTx_PT_CAN_ECU_GeneralKeepAwake_0x700_2_3E_80_55();
            // Serial.println();
            // Serial.print("canTx_PT_CAN_ECU_GeneralKeepAwake_0x700_2_3E_80  Sent  ");  Serial.print("\t"); 
            // Serial.println();

            loopDelayPT_CAN_KeepAwake_0x700 = 0;

          }
      
      }


    #pragma endregion


    #pragma region // Prepare and Send CAN Tx PT_CAN Messages - DTC Clearing 

        #pragma region // If conditions allow AND jobs are triggered, execute the following jobs: 

          #pragma region // DTC Clear Codes on PT-CAN Modules Only - KitchenSink Module - Sent in via PT


          #ifdef Concept_KitchenSink_NoSIC
              if (PT_CAN_DTC_Clear_Mode_Active == 1 && PT_CAN_DTC_Clear_Mode == 0 && CruiseStalk_SET_bit == 1 && loopDelayPT_CAN_DTC_Clear > 3000)  // Send PT-CAN DTC Clear Message
                {
                  canTx_PT_CAN_ECU_GeneralKeepAwake_0x7E0_2_3E_00();
                  canTx_PT_CAN_ECU_GeneralKeepAwake_0x7E0_2_10_3();

                  canTx_PT_CAN_22_0x700_ECU_DTC_Clear_4_14_FF(); // Send Initial Activation signal

                      GaugeTimer1 = 0;

                      loopDelayPT_CAN_DTC_Clear = 0;

                  
                }

            #endif



          #ifdef Concept_KitchenSink_SIC_Separate
              if (PT_CAN_DTC_Clear_Mode_Active == 1 && PT_CAN_DTC_Clear_Mode == 0 && CruiseStalk_SET_bit == 1 && loopDelayPT_CAN_DTC_Clear > 3000)  // Send PT-CAN DTC Clear Message
                {
                  canTx_PT_CAN_ECU_GeneralKeepAwake_0x7E0_2_3E_00();
                  canTx_PT_CAN_ECU_GeneralKeepAwake_0x7E0_2_10_3();

                  canTx_PT_CAN_22_0x700_ECU_DTC_Clear_4_14_FF(); // Send Initial Activation signal

                      GaugeTimer1 = 0;

                      loopDelayPT_CAN_DTC_Clear = 0;

                  
                }

            #endif

          #pragma endregion

  #ifdef BasicDAZA_v2024x1  // DTC Clear Codes on PT-CAN Modules Only - BasicDAZA

            #pragma region // DTC Clear Codes on PT-CAN Modules Only - BasicDAZA
            
              if (PT_CAN_DTC_Clear_Mode_Active == 1 && CruiseStalk_SET_bit == 1 && loopDelayPT_CAN_DTC_Clear > 3000 && DTC_Clear_Msg_Sequence == 0)  // Send PT-CAN DTC Clear Message
                {
                      canTx_PT_CAN_GeneralKeepAwake_0x700_2_3E_80_00();
                      canTx_PT_CAN_ECU_GeneralKeepAwake_0x700_2_10_3_00();
                      // canTx_PT_CAN_22_0x700_ECU_DTC_Clear_1_4_55_55();
                      GaugeTimer1 = 0;
                      loopDelayPT_CAN_DTC_Clear = 0;
                  DTC_Clear_Msg_Sequence = 1;
                }

              if (PT_CAN_DTC_Clear_Mode_Active == 1 && loopDelayPT_CAN_DTC_Clear < 302 && loopDelayPT_CAN_DTC_Clear > 300 && DTC_Clear_Msg_Sequence == 1)  // Send PT-CAN DTC Clear Message
                {
                  canTx_PT_CAN_22_0x700_ECU_DTC_Clear_4_14_FF(); 
                  DTC_Clear_Msg_Sequence = 2; 
                }


              if (PT_CAN_DTC_Clear_Mode_Active == 1 && loopDelayPT_CAN_DTC_Clear < 1002 && loopDelayPT_CAN_DTC_Clear > 1000 && DTC_Clear_Msg_Sequence == 2)  // Send PT-CAN DTC Clear Message
                {
                  canTx_PT_CAN_GeneralKeepAwake_0x700_2_3E_80_00();            
                  DTC_Clear_Msg_Sequence = 3;    
                }

              if (PT_CAN_DTC_Clear_Mode_Active == 1 && loopDelayPT_CAN_DTC_Clear < 1302 && loopDelayPT_CAN_DTC_Clear > 1300 && DTC_Clear_Msg_Sequence == 3)  // Send PT-CAN DTC Clear Message
                {
                  // canTx_PT_CAN_22_0x700_ECU_DTC_Clear_1_4_00();
                  canTx_PT_CAN_22_0x700_ECU_DTC_Clear_1_4_55_55();
                  DTC_Clear_Msg_Sequence = 4;
                }

              if (PT_CAN_DTC_Clear_Mode_Active == 1 && loopDelayPT_CAN_DTC_Clear < 2002 && loopDelayPT_CAN_DTC_Clear > 2000 && DTC_Clear_Msg_Sequence == 4)  // Send PT-CAN DTC Clear Message
                {
                  canTx_PT_CAN_GeneralKeepAwake_0x700_2_3E_80_00();   
                  DTC_Clear_Msg_Sequence = 0;             
                }


              #pragma endregion
  #endif

            #pragma endregion

    #pragma endregion

    #pragma region // Prepare and Send CAN Tx PT_CAN Messages - DQ500 Reset Requests

      #pragma region // Execute specific DQ500 Reset Functions

          if (DQ500Reset_ClutchCharacteristicCurve_HasRun == false && DQ500Reset_ClutchCharacteristic_AcceptableConditions == 1 && DQ500Reset_ClutchCharacteristicCurve_Allowed == 1 && CruiseStalk_SET_bit == 1)  // Reset Clutch Characteristic Curve
          {
            DQ500Reset_ClutchCharacteristicCurve_HasRun = true;
            // if(PT_CAN_STAT_FC_22_0x7E9 ==0){
            canTx_PT_CAN_22_0x7E1_DQ500_ClutchCharacteristicCurve_Reset(); // Send Initial Reset signal
                    Serial.print("canTx_PT_CAN_22_0x7E1_DQ500_ClutchCharacteristicCurve_Reset Sent  ");  Serial.print("\t"); 
                    //  Serial.print("PT_CAN_STAT_FC_22_0x7E9: "); Serial.print(PT_CAN_STAT_FC_22_0x7E9);
                GaugeTimer1 = 0;

          }
            // }
          if (DQ500Reset_DblClutchFastAdaptation_HasRun == false && DQ500Reset_DblClutchAdaptations_AcceptableConditions == 1 && DQ500Reset_DblClutchFastAdaptation_Allowed ==1 && CruiseStalk_SET_bit == 1)  // Reset Double Clutch Fast Adaptation
          {
            DQ500Reset_DblClutchFastAdaptation_HasRun = true;
        //    if(PT_CAN_STAT_FC_22_0x7E9 ==0){
            canTx_PT_CAN_22_0x7E1_DQ500_DblClutchFastAdaptation_Reset(); // Send Initial Reset signal
                    Serial.print("canTx_PT_CAN_22_0x7E1_DQ500_DblClutchFastAdaptation_Reset Sent  ");  Serial.print("\t"); 
                    //  Serial.print("PT_CAN_STAT_FC_22_0x7E9: "); Serial.print(PT_CAN_STAT_FC_22_0x7E9);
                
                GaugeTimer1 = 0;

          }
            // }
          if (DQ500Reset_ResetBasicSettings_HasRun == false && DQ500Reset_BasicSettings_AcceptableConditions ==1 && DQ500Reset_ResetBasicSettings_Allowed ==1 && CruiseStalk_SET_bit == 1)  // Reset Basic Settings
          {
            DQ500Reset_ResetBasicSettings_HasRun = true;
            loopDelayBasicDelayfromStart = 0;
          //  if(PT_CAN_STAT_FC_22_0x7E9 ==0){
            canTx_PT_CAN_22_0x7E1_DQ500_BasicSettings_Reset(); // Send Initial Reset signal
                    Serial.print("canTx_PT_CAN_22_0x7E1_DQ500_BasicSettings_Reset Sent  ");  Serial.print("\t"); 
                    //  Serial.print("PT_CAN_STAT_FC_22_0x7E9: "); Serial.print(PT_CAN_STAT_FC_22_0x7E9);
            GaugeTimer3 = 0;       
                
          }
            // }

      if(PT_CAN_STAT_FC_22_0x7E9 ==1){
            // delay(5);
                //  Serial.print(" ");  
                        canTx_PT_CAN_22_0x7E1_DQ500_FlowControl(); // Send FlowControl signal
                  Serial.print("canTx_PT_CAN_22_0x7E1_DQ500_BasicSettings_Reset_FlowControl Sent");  
                  //  Serial.print("\t"); 
                  //  Serial.print("PT_CAN_STAT_FC_22_0x7E9: ");
              }

      #pragma endregion

    #pragma endregion


  #pragma endregion



  #pragma region // CAN Message Preparations and Transmits - OEM MQB Instrument Cluster Overrides and Replacements

    #pragma region // CAN Frame Creations - OEM MQB Instrument Cluster Overrides and Replacements







        CAN_message_t PT_CAN_msg0x147;
    PT_CAN_msg0x147.len = 8;
    PT_CAN_msg0x147.id = 0x147;
    PT_CAN_msg0x147.buf[0] = 00; // 8
    PT_CAN_msg0x147.buf[1] = 00; // 8
    PT_CAN_msg0x147.buf[2] = 00; // 9
    PT_CAN_msg0x147.buf[3] = 00; // 9
    PT_CAN_msg0x147.buf[4] = 240; // 10
    PT_CAN_msg0x147.buf[5] = lsbDisplayGaugeTVal_Direct_0x147_byte5_Full; // 10
    PT_CAN_msg0x147.buf[6] = lsbDisplayGaugePVal_Direct__0x147_byte6_bits0to3 + msbDisplayGaugeTVal_Direct_0x147_byte6_bits4to7 ; // 11
    PT_CAN_msg0x147.buf[7] = msbDisplayGaugePVal_Direct_0x147_byte7_Full; // 11



        CAN_message_t PT_CAN_msg0x107; // Tachometer Override on Instrument Cluster 
    PT_CAN_msg0x107.len = 8;
    PT_CAN_msg0x107.id = 0x107;
    PT_CAN_msg0x107.buf[0] = 00; //
    PT_CAN_msg0x107.buf[1] = 00; //
    PT_CAN_msg0x107.buf[2] = 254; //
    PT_CAN_msg0x107.buf[3] = lsbMQB_DisplayTachometerOverride_Converted_FinalInt; //
    PT_CAN_msg0x107.buf[4] = msbMQB_DisplayTachometerOverride_Converted_FinalInt; //
    PT_CAN_msg0x107.buf[5] = 00; //
    PT_CAN_msg0x107.buf[6] = 00; //
    PT_CAN_msg0x107.buf[7] = 00; //


    //     CAN_message_t PT_CAN_msg0x107; // Tachometer Override on Instrument Cluster 
    // PT_CAN_msg0x107.len = 8;
    // PT_CAN_msg0x107.id = 0x107;
    // PT_CAN_msg0x107.buf[0] = 00; //
    // PT_CAN_msg0x107.buf[1] = 00; //
    // PT_CAN_msg0x107.buf[2] = 254; //
    // PT_CAN_msg0x107.buf[3] = 11; //
    // PT_CAN_msg0x107.buf[4] = 04; //
    // PT_CAN_msg0x107.buf[5] = 00; //
    // PT_CAN_msg0x107.buf[6] = 00; //
    // PT_CAN_msg0x107.buf[7] = 00; //



#pragma endregion

    #pragma region // CAN Message Transmits to OEM MQB Instrument Cluster Overrides and replacements


      if (DisplayTachOverride_Active == 1 ) { // send tach override to 0x107

        if (PT_CAN_canTxInterval_0x107 > MQB_GaugeDisplayOverride_TxRate_micros) { // this is in micros

            TFTCAN3.write(PT_CAN_msg0x107); //  CAN Frame for A0, A1, A2, A3
            PT_CAN_canTxInterval_0x107 = 0;
            // Serial.print("TachOverride Sent: "); Serial.println("");

          }
          
      }


//         if (CruiseStalk_UP_bit == 1 && WhlSpdRR == 0) { // Send DisplayGauge Override to 0x147

//           if (PT_CAN_canTxInterval_0x147 > 500) { // this is in Micros

//               TFTCAN3.write(PT_CAN_msg0x147); //  CAN Frame 
//       // TFTCAN3.write(PT_CAN_msg0x107); //  CAN Frame
//       PT_CAN_canTxInterval_0x147 = 0;
//       // Serial.print("GaugeOverride Sent: "); Serial.println("");

//              }
// }


        if (ConfigurationUpdateMode1_Active == 1 ) { // Send DisplayGauge Override to 0x147

          if (PT_CAN_canTxInterval_0x147 > MQB_GaugeDisplayOverride_TxRate_micros) { // this is in Micros

              TFTCAN3.write(PT_CAN_msg0x147); //  CAN Frame 
              PT_CAN_canTxInterval_0x147 = 0;
              // Serial.print("GaugeOverride Sent: "); Serial.println("");

            }
          }

//         if (CruiseStalk_DOWN_bit == 1 ) { // Send DisplayGauge Override to 0x147

//   if (PT_CAN_canTxInterval_0x147 > 200) { // this is in Micros

//       TFTCAN3.write(PT_CAN_msg0x147); //  CAN Frame 
//       // TFTCAN3.write(PT_CAN_msg0x107); //  CAN Frame
//       PT_CAN_canTxInterval_0x147 = 0;
//       // Serial.print("GaugeOverride Sent: "); Serial.println("");

//      }
// }

        else {
        digitalWrite(LED_PIN_CANDelay, HIGH);
      }

      #pragma endregion

  #pragma endregion




  }

void do_MQB_Paddle_SignalGeneration(){

#pragma region // Determine whether conditions/Gear Selector Position/Manual mode status meets acceptable status to execute PaddleUP Signal Generation

// if(GearLeverPosition_TIP == 1 && MQB_AutoShift_via_PaddleSignalGeneration_Allowed == 1) {


if(GearLeverPosition_TIP == 1 && MQB_AutoShift_via_PaddleSignalGeneration_Allowed == 1 && PaddleUP_SignalDebounceDelay > 300 && TimeSinceWhlSpd0 > 500 && DQ500_ClutchLockStatus == 128) {
  MQB_AutoShift_via_PaddleSignalGeneration_AcceptableCondition= 1;
}
else {
    MQB_AutoShift_via_PaddleSignalGeneration_AcceptableCondition = 0;
}

 #pragma endregion

#pragma region // Original/Simple Auto-UPshift via PaddleUP Signal Generation Functions

  #pragma region // PaddleUP Trigger RPM based on Shift RPM minus offset + multiplier

    // MQB_AutoUPshift_Gear1to2_RPMSetpoint_Trigger_Float = ( MQB_AutoUPshift_Gear1to2_RPMSetpoint_Int - ( (MQB_AutoUPshift_Gear1to2_RPMSetpoint_Int * MQB_AutoShift_EngineRPM_KalmanFilteredMPC_Gain_Float) / 0.5 )  );
    // MQB_AutoUPshift_Gear2to3_RPMSetpoint_Trigger_Float = ( MQB_AutoUPshift_Gear2to3_RPMSetpoint_Int - ( (MQB_AutoUPshift_Gear2to3_RPMSetpoint_Int * MQB_AutoShift_EngineRPM_KalmanFilteredMPC_Gain_Float) / 1.05   ) );
    // MQB_AutoUPshift_Gear3to4_RPMSetpoint_Trigger_Float = ( MQB_AutoUPshift_Gear3to4_RPMSetpoint_Int - ( (MQB_AutoUPshift_Gear3to4_RPMSetpoint_Int * MQB_AutoShift_EngineRPM_KalmanFilteredMPC_Gain_Float) / 2.75 ) );
    // MQB_AutoUPshift_Gear4to5_RPMSetpoint_Trigger_Float = ( MQB_AutoUPshift_Gear4to5_RPMSetpoint_Int - ( (MQB_AutoUPshift_Gear4to5_RPMSetpoint_Int * MQB_AutoShift_EngineRPM_KalmanFilteredMPC_Gain_Float) / 4   ) );
    // MQB_AutoUPshift_Gear5to6_RPMSetpoint_Trigger_Float = ( MQB_AutoUPshift_Gear5to6_RPMSetpoint_Int - ( (MQB_AutoUPshift_Gear5to6_RPMSetpoint_Int * MQB_AutoShift_EngineRPM_KalmanFilteredMPC_Gain_Float) / 16  ) );
    // MQB_AutoUPshift_Gear6to7_RPMSetpoint_Trigger_Float = ( MQB_AutoUPshift_Gear6to7_RPMSetpoint_Int - ( (MQB_AutoUPshift_Gear6to7_RPMSetpoint_Int * MQB_AutoShift_EngineRPM_KalmanFilteredMPC_Gain_Float) / 16  ) );

    //   if (DQ500_CurrentGear_0x394 == 1) {
    //     MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Float = MQB_AutoUPshift_Gear1to2_RPMSetpoint_Trigger_Float;
    // }
    // if (DQ500_CurrentGear_0x394 == 2) {
    //     MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Float = MQB_AutoUPshift_Gear2to3_RPMSetpoint_Trigger_Float;
    // }
    // if (DQ500_CurrentGear_0x394 == 3) {
    //     MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Float = MQB_AutoUPshift_Gear3to4_RPMSetpoint_Trigger_Float;
    // }
    // if (DQ500_CurrentGear_0x394 == 4) {
    //     MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Float = MQB_AutoUPshift_Gear4to5_RPMSetpoint_Trigger_Float;
    // }
    // if (DQ500_CurrentGear_0x394 == 5) {
    //     MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Float = MQB_AutoUPshift_Gear5to6_RPMSetpoint_Trigger_Float;
    // }
    // if (DQ500_CurrentGear_0x394 == 6) {
    //     MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Float = MQB_AutoUPshift_Gear6to7_RPMSetpoint_Trigger_Float;
    // }


  #pragma endregion

#pragma endregion

#pragma region // New Auto-UPshift via PaddleUP Signal Generation based on Predicted EngineRPM vs Offset

  if (MQB_AutoShift_via_PaddleSignalGeneration_AcceptableCondition == 1 ) { //  Upshifts 


  // MQB_AutoUPshift_Gear1to2_RPMSetpoint_Int = MQB_AutoUPshift_Gear1to2_RPMSetpoint_Trigger_Float;
  // MQB_AutoUPshift_Gear2to3_RPMSetpoint_Int = MQB_AutoUPshift_Gear2to3_RPMSetpoint_Trigger_Float;
  // MQB_AutoUPshift_Gear3to4_RPMSetpoint_Int = MQB_AutoUPshift_Gear3to4_RPMSetpoint_Trigger_Float;
  // MQB_AutoUPshift_Gear4to5_RPMSetpoint_Int = MQB_AutoUPshift_Gear4to5_RPMSetpoint_Trigger_Float;
  // MQB_AutoUPshift_Gear5to6_RPMSetpoint_Int = MQB_AutoUPshift_Gear5to6_RPMSetpoint_Trigger_Float;
  // MQB_AutoUPshift_Gear6to7_RPMSetpoint_Int = MQB_AutoUPshift_Gear6to7_RPMSetpoint_Trigger_Float;


  #pragma region // Original Triggers using EngineRPM_0x783

    // if (DQ500_CurrentGear_0x394 == 1 && (EngineRPM_0x783 > MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Float) )  //
    //     {
    //       Serial.print("canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration Sent  ");  Serial.print("\t"); 
    //       PaddleUP_SignalGeneration_Active = 1;
    //       canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
    //               Serial.print("canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration Sent  ");  Serial.print("\t"); 
    //               PaddleUP_SignalDebounceDelay = 0;
    //     }
    //   else {  PaddleUP_SignalGeneration_Active = 0;  }

    // if (DQ500_CurrentGear_0x394 == 2 && (EngineRPM_0x783 > MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Float)  )  //
    //     {
    //       PaddleUP_SignalGeneration_Active = 1;
    //       canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
    //               Serial.print("canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration Sent  ");  Serial.print("\t"); 
    //               PaddleUP_SignalDebounceDelay = 0;
    //     }
    //   else {  PaddleUP_SignalGeneration_Active = 0;  }

    // if (DQ500_CurrentGear_0x394 == 3 && (EngineRPM_0x783 > MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Float)  )  //
    //     {
    //       PaddleUP_SignalGeneration_Active = 1;
    //       canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
    //               Serial.print("canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration Sent  ");  Serial.print("\t"); 
    //               PaddleUP_SignalDebounceDelay = 0;
    //     }
    //   else {  PaddleUP_SignalGeneration_Active = 0;  }

    // if (DQ500_CurrentGear_0x394 == 4 && (EngineRPM_0x783 > MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Float)  )  //
    //     {
    //       PaddleUP_SignalGeneration_Active = 1;
    //       canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
    //               Serial.print("canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration Sent  ");  Serial.print("\t"); 
    //               PaddleUP_SignalDebounceDelay = 0;
    //     }
    //   else {  PaddleUP_SignalGeneration_Active = 0;  }

    // if (DQ500_CurrentGear_0x394 == 5 && (EngineRPM_0x783 > MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Float)  )  //
    //     {
    //       PaddleUP_SignalGeneration_Active = 1;
    //       canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
    //               Serial.print("canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration Sent  ");  Serial.print("\t"); 
    //               PaddleUP_SignalDebounceDelay = 0;
    //     }
    //   else {  PaddleUP_SignalGeneration_Active = 0;  }

    // if (DQ500_CurrentGear_0x394 == 6 && (EngineRPM_0x783 > MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Float)  )  //
    //     {
    //       PaddleUP_SignalGeneration_Active = 1;
    //       canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
    //               Serial.print("canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration Sent  ");  Serial.print("\t"); 
    //               PaddleUP_SignalDebounceDelay = 0;
    //     }
    //   else {  PaddleUP_SignalGeneration_Active = 0;  }
      

      #pragma endregion


#pragma endregion

    #pragma region // Calculate CurrentGear_RPMSetpoint_Trigger based on CurrentGear and RPM Setpoints from EEPROM

      if (DQ500_CurrentGear_0x394 == 1) {
          MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Float = MQB_AutoUPshift_Gear1to2_RPMSetpoint_Int;
      }
      if (DQ500_CurrentGear_0x394 == 2) {
          MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Float = MQB_AutoUPshift_Gear2to3_RPMSetpoint_Int;
      }
      if (DQ500_CurrentGear_0x394 == 3) {
          MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Float = MQB_AutoUPshift_Gear3to4_RPMSetpoint_Int;
      }
      if (DQ500_CurrentGear_0x394 == 4) {
          MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Float = MQB_AutoUPshift_Gear4to5_RPMSetpoint_Int;
      }
      if (DQ500_CurrentGear_0x394 == 5) {
          MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Float = MQB_AutoUPshift_Gear5to6_RPMSetpoint_Int;
      }
      if (DQ500_CurrentGear_0x394 == 6) {
          MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Float = MQB_AutoUPshift_Gear6to7_RPMSetpoint_Int;
      }


      MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Int = (MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Float/1);
      MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Intchar = MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Int;

    #pragma endregion

    #pragma region // New PaddleUP Signal Generation Executables using KalmanFiltered Predicted_EngRPM_200ms 

 #ifdef Concept_KitchenSink_SIC_Separate // When using RPM reference from SIC, the values are raw high-resolution measurements so offsets are applied

  if (DQ500_CurrentGear_0x394 == 1 && (TFT_KalmanFilteredMPC_PredictedEngRPM_Extrapolated200ms > MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Float - 200) )  //
      {
        Serial.print("canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration 1-2 Shift First Instruction Sent  ");  Serial.println("\t"); 
        PaddleUP_SignalGeneration_Active = 1;
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        Serial.print("canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration 1-2 Shift Last Instruction Sent  ");  Serial.print("\t"); 
                PaddleUP_SignalDebounceDelay = 0;
      }
    else {  PaddleUP_SignalGeneration_Active = 0;  }

  if (DQ500_CurrentGear_0x394 == 2 && (TFT_KalmanFilteredMPC_PredictedEngRPM_Extrapolated200ms > MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Float - 100)  )  //
      {
        Serial.print("canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration 2-3 Shift First Instruction Sent  ");  Serial.println("\t");         
        PaddleUP_SignalGeneration_Active = 1;
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        Serial.print("canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration 2-3 Shift Last Instruction Sent  ");  Serial.print("\t"); 
                PaddleUP_SignalDebounceDelay = 0;
      }
    else {  PaddleUP_SignalGeneration_Active = 0;  }

  if (DQ500_CurrentGear_0x394 == 3 && (TFT_KalmanFilteredMPC_PredictedEngRPM_Extrapolated200ms > MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Float)  )  //
      {
        Serial.print("canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration 3-4 Shift First Instruction Sent  ");  Serial.println("\t");         
        PaddleUP_SignalGeneration_Active = 1;
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        Serial.print("canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration 3-4 Shift Last Instruction Sent  ");  Serial.print("\t"); 
                PaddleUP_SignalDebounceDelay = 0;
      }
    else {  PaddleUP_SignalGeneration_Active = 0;  }

  if (DQ500_CurrentGear_0x394 == 4 && (TFT_KalmanFilteredMPC_PredictedEngRPM_Extrapolated200ms > MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Float)  )  //
      {
        Serial.print("canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration 4-5 Shift First Instruction Sent  ");  Serial.println("\t");         
        PaddleUP_SignalGeneration_Active = 1;
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        Serial.print("canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration 4-5 Shift Last Instruction Sent  ");  Serial.print("\t"); 
                PaddleUP_SignalDebounceDelay = 0;
      }
    else {  PaddleUP_SignalGeneration_Active = 0;  }

  if (DQ500_CurrentGear_0x394 == 5 && (TFT_KalmanFilteredMPC_PredictedEngRPM_Extrapolated200ms > MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Float)  )  //
      {
        Serial.print("canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration 5-6 Shift First Instruction Sent  ");  Serial.println("\t");         
        PaddleUP_SignalGeneration_Active = 1;
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        Serial.print("canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration 5-6 Shift Last Instruction Sent  ");  Serial.print("\t"); 
                PaddleUP_SignalDebounceDelay = 0;
      }
    else {  PaddleUP_SignalGeneration_Active = 0;  }

  if (DQ500_CurrentGear_0x394 == 6 && (TFT_KalmanFilteredMPC_PredictedEngRPM_Extrapolated200ms > MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Float)  )  //
      {
        Serial.print("canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration 6-7 Shift First Instruction Sent  ");  Serial.println("\t");         
        PaddleUP_SignalGeneration_Active = 1;
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        Serial.print("canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration 6-7 Shift Last Instruction Sent  ");  Serial.print("\t"); 
                PaddleUP_SignalDebounceDelay = 0;
      }
    else {  PaddleUP_SignalGeneration_Active = 0;  }
    
 #endif   

 #ifndef Concept_KitchenSink_SIC_Separate // When using RPM reference from 0x0A8, the values are filtered/ECU predicted so the offst is slightly different 

  if (DQ500_CurrentGear_0x394 == 1 && (TFT_KalmanFilteredMPC_PredictedEngRPM_Extrapolated200ms > MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Float ) )  //
      {
        Serial.print("canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration 1-2 Shift First Instruction Sent  ");  Serial.println("\t"); 
        PaddleUP_SignalGeneration_Active = 1;
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        Serial.print("canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration 1-2 Shift Last Instruction Sent  ");  Serial.print("\t"); 
                PaddleUP_SignalDebounceDelay = 0;
      }
    else {  PaddleUP_SignalGeneration_Active = 0;  }

  if (DQ500_CurrentGear_0x394 == 2 && (TFT_KalmanFilteredMPC_PredictedEngRPM_Extrapolated200ms > MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Float )  )  //
      {
        Serial.print("canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration 2-3 Shift First Instruction Sent  ");  Serial.println("\t");         
        PaddleUP_SignalGeneration_Active = 1;
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        Serial.print("canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration 2-3 Shift Last Instruction Sent  ");  Serial.print("\t"); 
                PaddleUP_SignalDebounceDelay = 0;
      }
    else {  PaddleUP_SignalGeneration_Active = 0;  }

  if (DQ500_CurrentGear_0x394 == 3 && (TFT_KalmanFilteredMPC_PredictedEngRPM_Extrapolated200ms > MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Float)  )  //
      {
        Serial.print("canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration 3-4 Shift First Instruction Sent  ");  Serial.println("\t");         
        PaddleUP_SignalGeneration_Active = 1;
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        Serial.print("canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration 3-4 Shift Last Instruction Sent  ");  Serial.print("\t"); 
                PaddleUP_SignalDebounceDelay = 0;
      }
    else {  PaddleUP_SignalGeneration_Active = 0;  }

  if (DQ500_CurrentGear_0x394 == 4 && (TFT_KalmanFilteredMPC_PredictedEngRPM_Extrapolated200ms > MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Float)  )  //
      {
        Serial.print("canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration 4-5 Shift First Instruction Sent  ");  Serial.println("\t");         
        PaddleUP_SignalGeneration_Active = 1;
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        Serial.print("canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration 4-5 Shift Last Instruction Sent  ");  Serial.print("\t"); 
                PaddleUP_SignalDebounceDelay = 0;
      }
    else {  PaddleUP_SignalGeneration_Active = 0;  }

  if (DQ500_CurrentGear_0x394 == 5 && (TFT_KalmanFilteredMPC_PredictedEngRPM_Extrapolated200ms > MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Float)  )  //
      {
        Serial.print("canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration 5-6 Shift First Instruction Sent  ");  Serial.println("\t");         
        PaddleUP_SignalGeneration_Active = 1;
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        Serial.print("canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration 5-6 Shift Last Instruction Sent  ");  Serial.print("\t"); 
                PaddleUP_SignalDebounceDelay = 0;
      }
    else {  PaddleUP_SignalGeneration_Active = 0;  }

  if (DQ500_CurrentGear_0x394 == 6 && (TFT_KalmanFilteredMPC_PredictedEngRPM_Extrapolated200ms > MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Float)  )  //
      {
        Serial.print("canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration 6-7 Shift First Instruction Sent  ");  Serial.println("\t");         
        PaddleUP_SignalGeneration_Active = 1;
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        Serial.print("canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration 6-7 Shift Last Instruction Sent  ");  Serial.print("\t"); 
                PaddleUP_SignalDebounceDelay = 0;
      }
    else {  PaddleUP_SignalGeneration_Active = 0;  }
    
 #endif   



    }
    
// Handle unsuccessful/unrecognized messages

  if (GearLeverPosition_TIP == 1 && 
      DQ500_CurrentGear_0x394 < 6 &&
      MQB_AutoShift_via_PaddleSignalGeneration_Allowed == 1 && 
      PaddleUP_SignalDebounceDelay > 290 && 
      WhlSpdAvg_Float > 50.0 &&
      temp_EngRPM > MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Float 
      ) {

        Serial.print("canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration FOLLOWUP Instructions Sent  ");  Serial.println("\t"); 
        PaddleUP_SignalGeneration_Active = 1;
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
        Serial.print("canTx_PT_MQB_0x3DD_PaddleUP_SignalGeneration FOLLOWUP Instructions Sent  ");  Serial.println("\t"); 
        PaddleUP_SignalDebounceDelay = 0;
      }

    else {  PaddleUP_SignalGeneration_Active = 0;  }


#pragma endregion



#pragma region // Auto-DOWNshift via PaddleDOWN Signal Generation based on current RPM vs Setpoints in EEPROM

  if (MQB_AutoShift_via_PaddleSignalGeneration_AcceptableCondition == 1 && ThrottleTPS_0x391 < 14  && (PaddleDOWN_SignalDebounceDelay > 750) ) { // Downshifts when Decelerating and when the Accel Pedal is 0


    if (DQ500_CurrentGear_0x394 == 2 && (TFT_KalmanFilteredMPC_PredictedEngRPM_Extrapolated200ms < MQB_AutoDOWNshift_Gear2to1_RPMSetpoint_Int) )  //
        {
          // PaddleDOWN_SignalGeneration_Active = 1;
          // canTx_PT_MQB_0x3DD_PaddleDOWN_SignalGeneration(); // Send PaddleUP Signal via 0x3DD
                  // Serial.print("canTx_PT_MQB_0x3DD_PaddleDOWN_SignalGeneration Sent  ");  Serial.println("\t"); 
                  // PaddleDOWN_SignalDebounceDelay = 0;
        }
      else {  PaddleDOWN_SignalGeneration_Active = 0;  }

    if (DQ500_CurrentGear_0x394 == 3 && (TFT_KalmanFilteredMPC_PredictedEngRPM_Extrapolated200ms < MQB_AutoDOWNshift_Gear3to2_RPMSetpoint_Int)  )  //
        {
          PaddleDOWN_SignalGeneration_Active = 1;
          canTx_PT_MQB_0x3DD_PaddleDOWN_SignalGeneration(); // Send PaddleDOWN Signal via 0x3DD
                  // Serial.print("canTx_PT_MQB_0x3DD_PaddleDOWN_SignalGeneration Sent  ");  Serial.println("\t"); 
                  PaddleDOWN_SignalDebounceDelay = 0;
        }
      else {  PaddleDOWN_SignalGeneration_Active = 0;  }

    if (DQ500_CurrentGear_0x394 == 4 && (TFT_KalmanFilteredMPC_PredictedEngRPM_Extrapolated200ms < MQB_AutoDOWNshift_Gear4to3_RPMSetpoint_Int)  )  //
        {
          PaddleDOWN_SignalGeneration_Active = 1;
          canTx_PT_MQB_0x3DD_PaddleDOWN_SignalGeneration(); // Send PaddleDOWN Signal via 0x3DD
                  // Serial.print("canTx_PT_MQB_0x3DD_PaddleDOWN_SignalGeneration Sent  ");  Serial.println("\t"); 
                  PaddleDOWN_SignalDebounceDelay = 0;
        }
      else {  PaddleDOWN_SignalGeneration_Active = 0;  }

    if (DQ500_CurrentGear_0x394 == 5 && (TFT_KalmanFilteredMPC_PredictedEngRPM_Extrapolated200ms < MQB_AutoDOWNshift_Gear5to4_RPMSetpoint_Int) )  //
        {
          PaddleDOWN_SignalGeneration_Active = 1;
          canTx_PT_MQB_0x3DD_PaddleDOWN_SignalGeneration(); // Send PaddleDOWN Signal via 0x3DD
                  // Serial.print("canTx_PT_MQB_0x3DD_PaddleDOWN_SignalGeneration Sent  ");  Serial.println("\t"); 
                  PaddleDOWN_SignalDebounceDelay = 0;
        }
      else {  PaddleDOWN_SignalGeneration_Active = 0;  }

    if (DQ500_CurrentGear_0x394 == 6 && (TFT_KalmanFilteredMPC_PredictedEngRPM_Extrapolated200ms < MQB_AutoDOWNshift_Gear6to5_RPMSetpoint_Int)  )  //
        {
          PaddleDOWN_SignalGeneration_Active = 1;
          canTx_PT_MQB_0x3DD_PaddleDOWN_SignalGeneration(); // Send PaddleDOWN Signal via 0x3DD
                  // Serial.print("canTx_PT_MQB_0x3DD_PaddleDOWN_SignalGeneration Sent  ");  Serial.println("\t"); 
                  PaddleDOWN_SignalDebounceDelay = 0;
        }
      else {  PaddleDOWN_SignalGeneration_Active = 0;  }

    if (DQ500_CurrentGear_0x394 == 7 && (TFT_KalmanFilteredMPC_PredictedEngRPM_Extrapolated200ms < MQB_AutoDOWNshift_Gear7to6_RPMSetpoint_Int)  )  //
        {
          PaddleDOWN_SignalGeneration_Active = 1;
          canTx_PT_MQB_0x3DD_PaddleDOWN_SignalGeneration(); // Send PaddleDOWN Signal via 0x3DD
                  // Serial.print("canTx_PT_MQB_0x3DD_PaddleDOWN_SignalGeneration Sent  ");  Serial.println("\t"); 
                  PaddleDOWN_SignalDebounceDelay = 0;
        }
      else {  PaddleDOWN_SignalGeneration_Active = 0;  }
      

    }

#pragma endregion

    
#pragma region  // Handle Debounce/Delay Timers for logging purposes

    if(PaddleUP_SignalDebounceDelay < 300) {
      PaddleUP_TimeSinceSignal = PaddleUP_SignalDebounceDelay;
    }
      else {
        PaddleUP_TimeSinceSignal = 0;
      }

    if(PaddleDOWN_SignalDebounceDelay < 250) {
      PaddleDOWN_TimeSinceSignal = PaddleDOWN_SignalDebounceDelay;
    }
      else {
        PaddleDOWN_TimeSinceSignal = 0;
      }

    #pragma endregion
    

#pragma endregion




}

void do_MQB_LaunchControl_TargetRPM_and_BoostWindow(){

 //  Bit and Decimal assignments for LaunchControl/TractionControl/BurnoutMode Statuses
 // 1 == LC RAMP Mode 
 // 2 == LC EngineRPM Transition Phase from Ramp to Hold, because BoostWindow EntryThreshold was Achieved
 // 4 == LC HOLD Mode, because BoostWindow EntryThreshold Achieved 
 // 8 == LC BoostWindow min Pressure was NOT Maintained
 // 16 == Burnout Mode Active
 // 32 == Fuel Overrun Mode Active
  


#pragma region // Determine whether LC Phase 0 BoostWindow is acheived and maintained. If so, update combined Status signal for CAN frame 
// BrakePedalSwitchStatus = digitalRead(5);
// WhlSpdRR = 0;
// temp_EngRPM = (analogRead(A0) * 2)+2000;


  #ifdef Concept_BasicDAZA
  temp_EngRPM = EngineRPM_0xA8;
  #endif

  #ifdef Concept_KitchenSink_NoSIC
  temp_EngRPM = EngineRPM_0xA8;
  #endif


  #ifdef Concept_KitchenSink_SIC_Separate
  temp_EngRPM = EngineRPM_0x788;
  #endif

  #ifdef Concept_KitchenSink_Sandals
  temp_EngRPM = measuredRPM;
  #endif




  if  (BrakePedalSwitchStatus == 1 && WhlSpdRR < 1 && temp_EngRPM > 1100 ) { // Allow for Launch Control operations, because VehSpeed is 0 and Brake is pressed
  // if  (BrakePedalSwitchStatus == 1 && WhlSpdRR < 1 ) { // Allow for Launch Control operations, because VehSpeed is 0 and Brake is pressed

    // Determine if EngineRPM RAMP mode should become active
    if( (MQB_LaunchControl_BoostWindow_Maintained_Status == 0) && (PressureA8 < MQB_LaunchControl_BoostWindow_EntryThresholdPressure_Int)) { 
      
      MQB_LaunchControl_BoostWindow_EntryThresholdPressure_Achieved = 0;

      MQB_LaunchControl_RampModeActiveStatus = 1;
      MQB_LaunchControl_TransitionModeActiveStatus = 0;
      MQB_LaunchControl_HoldModeActiveStatus = 0;

      MQB_TFT_EngineRPM_Setpoint_Int = MQB_LaunchControl_EngineRPM_Ramp_Setpoint_Int;
      MQB_LaunchControl_CombinedSignalActive = 1;

      }
      else { 
          MQB_LaunchControl_RampModeActiveStatus = 0;
        }


    // Transition from RAMP Mode to Transition Mode, once Boost Threshold has been achieved
    if(  (MQB_LaunchControl_BoostWindow_EntryThresholdPressure_Achieved == 0) && (PressureA8 > MQB_LaunchControl_BoostWindow_EntryThresholdPressure_Int) ) {
        
        MQB_LaunchControl_BoostWindow_EntryThresholdPressure_Achieved = 1;
        MQB_LaunchControl_EngineRPM_RAMP_Mode_Timer = 0;

        MQB_LaunchControl_RampModeActiveStatus = 0;
        MQB_LaunchControl_TransitionModeActiveStatus = 1;
        MQB_LaunchControl_HoldModeActiveStatus = 0;

        MQB_LaunchControl_CombinedSignalActive = 2;

      }

    // Confirm Boost Pressure is still within BoostWindow
    if(  (MQB_LaunchControl_BoostWindow_EntryThresholdPressure_Achieved == 1) && (PressureA8 > MQB_LaunchControl_BoostWindow_minMaintainPressure_Int) ) {
        MQB_LaunchControl_BoostWindow_Maintained_Status = 1;
      }
      else { // Exit BoostWindow_Maintained_Status
        MQB_LaunchControl_BoostWindow_Maintained_Status = 0;
      }


    // PT_CAN_CALculate/Update Target EngineRPM during Transition Mode, IF BoostWindow is still maintained and within the Transition Timeout
    if( (MQB_LaunchControl_BoostWindow_Maintained_Status == 1 && MQB_LaunchControl_TransitionModeActiveStatus == 1) && (MQB_LaunchControl_EngineRPM_RAMP_Mode_Timer < MQB_LaunchControl_EngineRPM_Hold_TransitionTimeout && temp_EngRPM > (MQB_LaunchControl_EngineRPM_Hold_Setpoint_Int+50) ) ) {

      MQB_LaunchControl_EngineRPM_TransitionRateMultiplier_Float = MQB_LaunchControl_EngineRPM_TransitionRateMultiplier_Int / 10.0;
      MQB_LaunchControl_EngineRPM_Transition_Setpoint_Int = ( MQB_LaunchControl_EngineRPM_Ramp_Setpoint_Int - (MQB_LaunchControl_EngineRPM_RAMP_Mode_Timer * MQB_LaunchControl_EngineRPM_TransitionRateMultiplier_Float) );
      MQB_TFT_EngineRPM_Setpoint_Int = MQB_LaunchControl_EngineRPM_Transition_Setpoint_Int;
      MQB_TFT_EngineRPM_Setpoint_Int = constrain(MQB_TFT_EngineRPM_Setpoint_Int,(MQB_LaunchControl_EngineRPM_Hold_Setpoint_Int - 0),MQB_LaunchControl_EngineRPM_Ramp_Setpoint_Int);

      MQB_LaunchControl_CombinedSignalActive = 2;

    }


    // Determine if EngineRPM HOLD mode should become active - Classic - When RPM drops to/below the HOLD RPM (or when transition timer expires), change the Target RPM to the HOLD RPM
    // if( MQB_LaunchControl_BoostWindow_Maintained_Status == 1 && MQB_LaunchControl_TransitionModeActiveStatus == 1 && (MQB_LaunchControl_EngineRPM_RAMP_Mode_Timer > MQB_LaunchControl_EngineRPM_Hold_TransitionTimeout || temp_EngRPM < (MQB_LaunchControl_EngineRPM_Hold_Setpoint_Int+50) ) ) {
    //   MQB_LaunchControl_RampModeActiveStatus = 0;
    //   MQB_LaunchControl_TransitionModeActiveStatus = 0;
    //   MQB_LaunchControl_HoldModeActiveStatus = 1;

    //   MQB_TFT_EngineRPM_Setpoint_Int = MQB_LaunchControl_EngineRPM_Hold_Setpoint_Int;
    //   MQB_TFT_EngineRPM_Setpoint_Int = constrain(MQB_TFT_EngineRPM_Setpoint_Int,MQB_LaunchControl_EngineRPM_Hold_Setpoint_Int,MQB_LaunchControl_EngineRPM_Ramp_Setpoint_Int);

    //   MQB_LaunchControl_CombinedSignalActive = 4;

    //   }


    // Determine if EngineRPM HOLD mode should become active - New - When RPM drops to/below the HOLD RPM (or when transition timer expires), Keep the existing Target RPM Setpoint from when the Actual RPM reached the HOLD RPM
    if( MQB_LaunchControl_BoostWindow_Maintained_Status == 1 && MQB_LaunchControl_TransitionModeActiveStatus == 1 && (temp_EngRPM < (MQB_LaunchControl_EngineRPM_Hold_Setpoint_Int + 100) ) ) {

      MQB_LaunchControl_RampModeActiveStatus = 0;
      MQB_LaunchControl_TransitionModeActiveStatus = 0;
      MQB_LaunchControl_HoldModeActiveStatus = 1;

      MQB_LaunchControl_EngineRPM_NewHold_Setpoint_Int = MQB_TFT_EngineRPM_Setpoint_Int;

      MQB_TFT_EngineRPM_Setpoint_Int = MQB_TFT_EngineRPM_Setpoint_Int;
      MQB_TFT_EngineRPM_Setpoint_Int = constrain(MQB_LaunchControl_EngineRPM_NewHold_Setpoint_Int,(MQB_LaunchControl_EngineRPM_Hold_Setpoint_Int- 0),MQB_LaunchControl_EngineRPM_NewHold_Setpoint_Int);

      MQB_LaunchControl_CombinedSignalActive = 4;

      }

  
    }


    MQB_LaunchControl_EngineRPM_Setpoint_CurrentDelta = (MQB_TFT_EngineRPM_Setpoint_Int - temp_EngRPM);
    MQB_LaunchControl_EngineRPM_Setpoint_CurrentDelta_16bitSigned = (MQB_TFT_EngineRPM_Setpoint_Int - temp_EngRPM) + 32768;
    MQB_LaunchControl_EngineRPM_Setpoint_CurrentDelta_8bit = (MQB_TFT_EngineRPM_Setpoint_Int / 20) - (temp_EngRPM / 20);

    MQB_IgnTimingCorrection_8bitSigned = MQB_LaunchControl_EngineRPM_Setpoint_CurrentDelta / 20;


      
      #pragma endregion


#pragma region // Create/Update Target EngineRPM and CombinedSignalActive value based on BurnoutMode, FuelOverrun Mode, etc etc

    // LaunchControl Phase 0 non-engagement if brake pedal is not pressed and not currently in burnout mode
    if (BrakePedalSwitchStatus == 0 && MQB_BurnoutMode_ActiveStatus == 0 ) {
      MQB_LaunchControl_RampModeActiveStatus = 0;
      MQB_LaunchControl_TransitionModeActiveStatus = 0;
      MQB_LaunchControl_HoldModeActiveStatus = 0;

      MQB_LaunchControl_BoostWindow_EntryThresholdPressure_Achieved = 0;
      MQB_LaunchControl_BoostWindow_Maintained_Status = 0;

      MQB_TFT_EngineRPM_Setpoint_Int =  MQB_DefaultEngRPMLimit ;
      MQB_TFT_EngineRPM_Setpoint_Int_8bit = ( MQB_DefaultEngRPMLimit / 100 );

      MQB_LaunchControl_CombinedSignalActive = 0;

    }

      // BurnoutMode Mode    
      else if ( MQB_BurnoutMode_ActiveStatus == 1 ) {

        MQB_TFT_EngineRPM_Setpoint_Int = MQB_BurnoutMode_EngineRPM_Setpoint_Int; 
        MQB_TFT_EngineRPM_Setpoint_Int_8bit = MQB_TFT_EngineRPM_Setpoint_Int / 100; 
        MQB_LaunchControl_CombinedSignalActive = 16;
        }

      // Fuel Overrun Mode       
      else if (WhlSpdRR > 50 && MQB_FuelOverrun_OverrideModeActive_Status == 1) { // Fuel Overrun Override Mode
          MQB_TFT_EngineRPM_Setpoint_Int_8bit = ( MQB_DefaultEngRPMLimit / 100 );
          MQB_LaunchControl_CombinedSignalActive = 32;
        }



  #pragma endregion





#pragma region // Create/Update BoostByGear Multiplier and Activation Signal value (bitshifted)

if( MQB_BoostByGearMultiplier_ActivationStatus_Int == 1) {
  
    if(( DQ500_CurrentGear_0x394 == 1 && WhlSpdAvg_Float < 1 ))
        { 
          lsbMQB_BoostByGearMultiplier_Current = MQB_BoostByGearMultiplier_Gear1_Int;
          lsbMQB_BoostByGearMultiplier_ActivationStatus_Int = MQB_BoostByGearMultiplier_ActivationStatus_Int;
        }



    if(( DQ500_CurrentGear_0x394 == 1 && DQ500_ClutchLockStatus == 128) || (DQ500_CurrentGear_0x394 == 1 && WhlSpdRR < 20))
        { 
          lsbMQB_BoostByGearMultiplier_Current = MQB_BoostByGearMultiplier_Gear1_Int;
          lsbMQB_BoostByGearMultiplier_ActivationStatus_Int = MQB_BoostByGearMultiplier_ActivationStatus_Int;
        }

    if( DQ500_CurrentGear_0x394 == 2 && DQ500_ClutchLockStatus == 128 )
        { 
          lsbMQB_BoostByGearMultiplier_Current = MQB_BoostByGearMultiplier_Gear2_Int;
          lsbMQB_BoostByGearMultiplier_ActivationStatus_Int = MQB_BoostByGearMultiplier_ActivationStatus_Int;
        }
  
    if( DQ500_CurrentGear_0x394 == 3 && DQ500_ClutchLockStatus == 128 )
        { 
          lsbMQB_BoostByGearMultiplier_Current = MQB_BoostByGearMultiplier_Gear3_Int;
          lsbMQB_BoostByGearMultiplier_ActivationStatus_Int = MQB_BoostByGearMultiplier_ActivationStatus_Int;
        }

    if( DQ500_CurrentGear_0x394 == 4 && DQ500_ClutchLockStatus == 128 )
        { 
          lsbMQB_BoostByGearMultiplier_Current = MQB_BoostByGearMultiplier_Gear4_Int;
          lsbMQB_BoostByGearMultiplier_ActivationStatus_Int = MQB_BoostByGearMultiplier_ActivationStatus_Int;
        }

    if( DQ500_CurrentGear_0x394 == 5 && DQ500_ClutchLockStatus == 128 )
        { 
          lsbMQB_BoostByGearMultiplier_Current = MQB_BoostByGearMultiplier_Gear5_Int;
          lsbMQB_BoostByGearMultiplier_ActivationStatus_Int = MQB_BoostByGearMultiplier_ActivationStatus_Int;
        }

    if( DQ500_CurrentGear_0x394 == 6 && DQ500_ClutchLockStatus == 128)
        { 
          lsbMQB_BoostByGearMultiplier_Current = MQB_BoostByGearMultiplier_Gear6_Int;
          lsbMQB_BoostByGearMultiplier_ActivationStatus_Int = MQB_BoostByGearMultiplier_ActivationStatus_Int;
        }

    if( DQ500_CurrentGear_0x394 == 7 && DQ500_ClutchLockStatus == 128 )
        { 
          lsbMQB_BoostByGearMultiplier_Current = MQB_BoostByGearMultiplier_Gear7_Int;
          lsbMQB_BoostByGearMultiplier_ActivationStatus_Int = MQB_BoostByGearMultiplier_ActivationStatus_Int;
        }

      }

      else{
          lsbMQB_BoostByGearMultiplier_Current = 100;
          lsbMQB_BoostByGearMultiplier_ActivationStatus_Int = MQB_BoostByGearMultiplier_ActivationStatus_Int;

      }

  #pragma endregion


#pragma region // Determine whether Burnout Mode should be activated, and if so, update Status signal for CAN frame

if(HazardLightsButton == 1) {
  MQB_BurnoutMode_ActiveStatus = 1;
}
  else {
    MQB_BurnoutMode_ActiveStatus = 0;
  }


  #pragma endregion

#pragma region // Determine whether Fuel Overrun Override Mode should be activated, and if so, update Status signal for CAN frame

if(temp_EngRPM > 1000 &&  AccelPedal_0x391 < 16 && (MFSW_LeftSide_BACK_Pressed == 1 || BOTH_Paddles_Pulled == 1 || CruiseStalk_UP_bit == 1)) {
  MQB_FuelOverrun_OverrideModeActive_Status = 1;
  // Serial.println("Fuel overrun override active");
}
  else {
    MQB_FuelOverrun_OverrideModeActive_Status = 0;
  }


  #pragma endregion

#pragma region // bitshifting of EngineRPM Setpoints and status variables for CAN frames

  // MQB_LaunchControl_EngineRPM_Setpoint_Int = 2600;
  
      lsbMQB_LaunchControl_EngineRPM_Ramp_Setpoint_Int = MQB_LaunchControl_EngineRPM_Ramp_Setpoint_Int & 0x00FF;
      msbMQB_LaunchControl_EngineRPM_Ramp_Setpoint_Int = (MQB_LaunchControl_EngineRPM_Ramp_Setpoint_Int >> 8) & 0x00FF;
  
      lsbMQB_LaunchControl_EngineRPM_Hold_Setpoint_Int = MQB_LaunchControl_EngineRPM_Hold_Setpoint_Int & 0x00FF;
      msbMQB_LaunchControl_EngineRPM_Hold_Setpoint_Int = (MQB_LaunchControl_EngineRPM_Hold_Setpoint_Int >> 8) & 0x00FF;
  
      lsbMQB_LaunchControl_EngineRPM_Transition_Setpoint_Int = MQB_LaunchControl_EngineRPM_Transition_Setpoint_Int & 0x00FF;
      msbMQB_LaunchControl_EngineRPM_Transition_Setpoint_Int = (MQB_LaunchControl_EngineRPM_Transition_Setpoint_Int >> 8) & 0x00FF;
  
      lsbMQB_LaunchControl_EngineRPM_Transition_Setpoint_Int = MQB_LaunchControl_EngineRPM_Transition_Setpoint_Int & 0x00FF;
      msbMQB_LaunchControl_EngineRPM_Transition_Setpoint_Int = (MQB_LaunchControl_EngineRPM_Transition_Setpoint_Int >> 8) & 0x00FF;



      MQB_LaunchControl_EngineRPM_Ramp_Setpoint_Int_8bit =  (MQB_LaunchControl_EngineRPM_Ramp_Setpoint_Int / 100);
      MQB_LaunchControl_EngineRPM_Hold_Setpoint_Int_8bit =  (MQB_LaunchControl_EngineRPM_Hold_Setpoint_Int / 100);
      MQB_LaunchControl_EngineRPM_Transition_Setpoint_Int_8bit =  (MQB_LaunchControl_EngineRPM_Transition_Setpoint_Int / 100);
  


      lsbMQB_LaunchControl_EngineRPM_Setpoint_CurrentDelta = MQB_LaunchControl_EngineRPM_Setpoint_CurrentDelta & 0x00FF;
      msbMQB_LaunchControl_EngineRPM_Setpoint_CurrentDelta = (MQB_LaunchControl_EngineRPM_Setpoint_CurrentDelta >> 8) & 0x00FF;

      // lsbMQB_LaunchControl_EngineRPM_Setpoint_CurrentDelta = MQB_LaunchControl_EngineRPM_Setpoint_CurrentDelta 
  // ----------------------------------------------------------------------------------------------------------------------------
  
      lsbMQB_LaunchControl_BoostWindow_EntryThresholdPressure_Int = MQB_LaunchControl_BoostWindow_EntryThresholdPressure_Int & 0x00FF;
      msbMQB_LaunchControl_BoostWindow_EntryThresholdPressure_Int = (MQB_LaunchControl_BoostWindow_EntryThresholdPressure_Int >> 8) & 0x00FF;
  
      lsbMQB_LaunchControl_BoostWindow_minMaintainPressure_Int = MQB_LaunchControl_BoostWindow_minMaintainPressure_Int & 0x00FF;
      msbMQB_LaunchControl_BoostWindow_minMaintainPressure_Int = (MQB_LaunchControl_BoostWindow_minMaintainPressure_Int >> 8) & 0x00FF;
  
      lsbMQB_LaunchControl_BoostVariableSelection_Int =  MQB_LaunchControl_BoostVariableSelection_Int & 0x00FF;
      msbMQB_LaunchControl_BoostVariableSelection_Int = (MQB_LaunchControl_BoostVariableSelection_Int >> 8) & 0x00FF;


  
      lsbMQB_LaunchControl_BoostWindow_EntryThresholdPressure_Int_8bit = (MQB_LaunchControl_BoostWindow_EntryThresholdPressure_Int / 100);
      lsbMQB_LaunchControl_BoostWindow_minMaintainPressure_Int_8bit = (MQB_LaunchControl_BoostWindow_minMaintainPressure_Int / 100);
      lsbMQB_LaunchControl_BoostVariableSelection_Int_8bit = (lsbMQB_LaunchControl_BoostVariableSelection_Int / 100);
  
  // ----------------------------------------------------------------------------------------------------------------------------
  
      lsbMQB_TractionControl_TgtSlipRate_Int = MQB_TractionControl_TgtSlipRate_Int & 0x00FF;
      msbMQB_TractionControl_TgtSlipRate_Int = (MQB_TractionControl_TgtSlipRate_Int >> 8) & 0x00FF;
  
  
      lsbMQB_TractionControl_CorrectionAggression_Int = MQB_TractionControl_CorrectionAggression_Int & 0x00FF;
      msbMQB_TractionControl_CorrectionAggression_Int = (MQB_TractionControl_CorrectionAggression_Int >> 8) & 0x00FF;
  
      lsbMQB_TractionControl_CorrectionAggression_Int_8bit = MQB_TractionControl_CorrectionAggression_Int;
  
  // ----------------------------------------------------------------------------------------------------------------------------
  
      lsbMQB_BurnoutMode_EngineRPM_Setpoint_Int = MQB_BurnoutMode_EngineRPM_Setpoint_Int & 0x00FF;
      msbMQB_BurnoutMode_EngineRPM_Setpoint_Int = (MQB_BurnoutMode_EngineRPM_Setpoint_Int >> 8) & 0x00FF;
  
      MQB_BurnoutMode_EngineRPM_Setpoint_Int_8bit = (MQB_BurnoutMode_EngineRPM_Setpoint_Int / 100);
  
  
   #pragma endregion
  
  


}

void do_TFTCAN_PT_CAN_Transmits_DS1(){
  digitalToggle(LED_PIN_CANTransmit);

#pragma region // CAN Frame Creations - DS1 Frames 0x780 to 0x787


  #pragma region // 0x780 - Basic TFT Module Operations and DisplayGauges

  #ifndef KitchenSink_2025x3_SIC_Separate

      CAN_message_t PT_CAN_msg0x780;
      PT_CAN_msg0x780.len = 8;
      PT_CAN_msg0x780.id = 0x780;
      PT_CAN_msg0x780.buf[0] = lsbInternalCPUTemp;   // 0
      PT_CAN_msg0x780.buf[1] = (lsbIgnitionAwake + (PaddleUP_TimeSinceSignal / 10) + (PaddleDOWN_TimeSinceSignal / 10));   // 0
      PT_CAN_msg0x780.buf[2] = DQ500_CurrentGear_0x394; // 1
      PT_CAN_msg0x780.buf[3] = MQB_BoostByGearMultiplier_ActivationStatus_Int * 2; // 1
      PT_CAN_msg0x780.buf[4] = lsbDisplayGaugePVal;  // 2
      PT_CAN_msg0x780.buf[5] = msbDisplayGaugePVal;  // 2
      PT_CAN_msg0x780.buf[6] = lsbDisplayGaugeTVal;        // 3
      PT_CAN_msg0x780.buf[7] = msbDisplayGaugeTVal;        // 3

  #endif

  #ifdef KitchenSink_2025x3_SIC_Separate

      CAN_message_t PT_CAN_msg0x780;
      PT_CAN_msg0x780.len = 8;
      PT_CAN_msg0x780.id = 0x780;
      PT_CAN_msg0x780.buf[0] = lsbInternalCPUTemp;   // 0
      PT_CAN_msg0x780.buf[1] = (lsbIgnitionAwake + (PaddleUP_TimeSinceSignal / 10) + (PaddleDOWN_TimeSinceSignal / 10));   // 0
      PT_CAN_msg0x780.buf[2] = DQ500_CurrentGear_0x394; // 1
      PT_CAN_msg0x780.buf[3] = MQB_LaunchControl_CombinedSignalActive; // 1
      PT_CAN_msg0x780.buf[4] = lsbDisplayGaugePVal;  // 2
      PT_CAN_msg0x780.buf[5] = msbDisplayGaugePVal;  // 2
      PT_CAN_msg0x780.buf[6] = lsbDisplayGaugeTVal;        // 3
      PT_CAN_msg0x780.buf[7] = msbDisplayGaugeTVal;        // 3

  #endif

  #pragma endregion

  #pragma region // 0x781 - TFT Module Configurations - (Launch Control, Traction Control, BBG, etc)  ***Multiplexing as the sample rate is not important 

      // original pre-august2025
    CAN_message_t PT_CAN_msg0x781;
    PT_CAN_msg0x781.len = 8;
    PT_CAN_msg0x781.id = 0x781;
    PT_CAN_msg0x781.buf[0] = (lowByte(LC_EngineRPM_PIDoutput_TotalIgnCorrection_sInt)); // 4
    PT_CAN_msg0x781.buf[1] = (highByte(LC_EngineRPM_PIDoutput_TotalIgnCorrection_sInt)); // 4
    PT_CAN_msg0x781.buf[2] = (lowByte(MQB_LaunchControl_EngineRPM_Setpoint_CurrentDelta)); // 5
    PT_CAN_msg0x781.buf[3] = (highByte(MQB_LaunchControl_EngineRPM_Setpoint_CurrentDelta)); // 5
    PT_CAN_msg0x781.buf[4] = (MQB_TFT_EngineRPM_Setpoint_Int / 100); // 6  - bits 0 to 7 which is 0xD0010D7A in 404G ECU
    PT_CAN_msg0x781.buf[5] = MQB_LaunchControl_CombinedSignalActive; // 6 - bits 8 to 15
    PT_CAN_msg0x781.buf[6] = lsbMQB_BoostByGearMultiplier_Current;  // 7 bits 0 to 7, which is 0xD0010D7C in 404G ECU
    PT_CAN_msg0x781.buf[7] = lsbMQB_TractionControl_CorrectionAggression_Int_8bit;  // 7



// new Mux5 setup
    // CAN_message_t PT_CAN_msg0x781_mux1;
    // PT_CAN_msg0x781_mux1.len = 8;
    // PT_CAN_msg0x781_mux1.id = 0x781;
    // PT_CAN_msg0x781_mux1.buf[0] = 1; // 4
    // PT_CAN_msg0x781_mux1.buf[1] = 0; // 4
    // PT_CAN_msg0x781_mux1.buf[2] = (lowByte(MQB_LaunchControl_EngineRPM_Setpoint_CurrentDelta)); //5
    // PT_CAN_msg0x781_mux1.buf[3] = (highByte(MQB_LaunchControl_EngineRPM_Setpoint_CurrentDelta)); //5
    // PT_CAN_msg0x781_mux1.buf[4] = lsbMQB_BoostByGearMultiplier_Current;; // 6  - bits 0 to 7
    // PT_CAN_msg0x781_mux1.buf[5] = MQB_BoostByGearMultiplier_ActivationStatus_Int * 2; // 6 - bits 8 to 15
    // PT_CAN_msg0x781_mux1.buf[6] = (MQB_TFT_EngineRPM_Setpoint_Int / 100);  // 7
    // PT_CAN_msg0x781_mux1.buf[7] = lsbMQB_TractionControl_CorrectionAggression_Int_8bit;  // 7


    // CAN_message_t PT_CAN_msg0x781_mux2;
    // PT_CAN_msg0x781_mux2.len = 8;
    // PT_CAN_msg0x781_mux2.id = 0x781;
    // PT_CAN_msg0x781_mux2.buf[0] = 2; // 4
    // PT_CAN_msg0x781_mux2.buf[1] = 0; // 4
    // PT_CAN_msg0x781_mux2.buf[2] = MQB_BoostByGearMultiplier_ActivationStatus_Int * 2; //1 //5
    // PT_CAN_msg0x781_mux2.buf[3] = lowByte(MQB_AutoShift_EngineRPM_KalmanFilteredMPC_Gain_Int); //20 //5
    // PT_CAN_msg0x781_mux2.buf[4] = lowByte(MQB_LaunchControl_EngineRPM_PID_Ki_Term_Int); //22 //6
    // PT_CAN_msg0x781_mux2.buf[5] = lowByte(MQB_LaunchControl_EngineRPM_PID_BaseOffset_Int); //22 //6
    // PT_CAN_msg0x781_mux2.buf[6] = lowByte(MQB_LaunchControl_EngineRPM_Ramp_Setpoint_Int / 100);  // 7
    // PT_CAN_msg0x781_mux2.buf[7] = lowByte(MQB_LaunchControl_EngineRPM_Hold_Setpoint_Int / 100);  // 7



    // CAN_message_t PT_CAN_msg0x781_mux3;
    // PT_CAN_msg0x781_mux3.len = 8;
    // PT_CAN_msg0x781_mux3.id = 0x781;
    // PT_CAN_msg0x781_mux3.buf[0] = 3; // 4
    // PT_CAN_msg0x781_mux3.buf[1] = 0; // 4
    // PT_CAN_msg0x781_mux3.buf[2] = lowByte(MQB_LaunchControl_BoostWindow_EntryThresholdPressure_Int / 100);  //5
    // PT_CAN_msg0x781_mux3.buf[3] = lowByte(MQB_LaunchControl_BoostWindow_minMaintainPressure_Int / 100);  //5 
    // PT_CAN_msg0x781_mux3.buf[4] = lowByte(MQB_LaunchControl_BoostWindow_EntryThresholdPressure_Achieved);    //6
    // PT_CAN_msg0x781_mux3.buf[5] = lowByte(MQB_LaunchControl_RampModeActiveStatus);    //6
    // PT_CAN_msg0x781_mux3.buf[6] = lowByte(MQB_LaunchControl_TransitionModeActiveStatus);    //7
    // PT_CAN_msg0x781_mux3.buf[7] = lowByte(MQB_LaunchControl_HoldModeActiveStatus);  //7



    // CAN_message_t PT_CAN_msg0x781_mux4;
    // PT_CAN_msg0x781_mux4.len = 8;
    // PT_CAN_msg0x781_mux4.id = 0x781;
    // PT_CAN_msg0x781_mux4.buf[0] = 4; // 4
    // PT_CAN_msg0x781_mux4.buf[1] = 0; // 4
    // PT_CAN_msg0x781_mux4.buf[2] = lowByte(MQB_BurnoutMode_ActiveStatus); // 5
    // PT_CAN_msg0x781_mux4.buf[3] = lowByte(MQB_BurnoutMode_EngineRPM_Setpoint_Int / 100); // 5
    // PT_CAN_msg0x781_mux4.buf[4] = MQB_LaunchControl_BoostWindow_Maintained_Status; // 6  - bits 0 to 7
    // PT_CAN_msg0x781_mux4.buf[5] = MQB_LaunchControl_CombinedSignalActive; // 6 - bits 8 to 15
    // PT_CAN_msg0x781_mux4.buf[6] = lsbMQB_BoostByGearMultiplier_Current;  // 7
    // PT_CAN_msg0x781_mux4.buf[7] = lsbMQB_TractionControl_CorrectionAggression_Int_8bit;  // 7



    // CAN_message_t PT_CAN_msg0x781_mux5;
    // PT_CAN_msg0x781_mux5.len = 8;
    // PT_CAN_msg0x781_mux5.id = 0x781;
    // PT_CAN_msg0x781_mux5.buf[0] = 5; // 4
    // PT_CAN_msg0x781_mux5.buf[1] = 0; // 4
    // PT_CAN_msg0x781_mux5.buf[2] = (lowByte(MQB_LaunchControl_EngineRPM_Setpoint_CurrentDelta)); // 5
    // PT_CAN_msg0x781_mux5.buf[3] = (highByte(MQB_LaunchControl_EngineRPM_Setpoint_CurrentDelta)); // 5
    // PT_CAN_msg0x781_mux5.buf[4] = (MQB_TFT_EngineRPM_Setpoint_Int / 100); // 6  - bits 0 to 7
    // PT_CAN_msg0x781_mux5.buf[5] = MQB_LaunchControl_CombinedSignalActive; // 6 - bits 8 to 15
    // PT_CAN_msg0x781_mux5.buf[6] = lsbMQB_BoostByGearMultiplier_Current;  // 7
    // PT_CAN_msg0x781_mux5.buf[7] = lsbMQB_TractionControl_CorrectionAggression_Int_8bit;  // 7
   

  #pragma endregion
  
  #pragma region // 0x782 - Primary Analog Inputs and TurboRPM

  #ifdef Concept_BasicDAZA
    CAN_message_t PT_CAN_msg0x782;
    PT_CAN_msg0x782.len = 8;
    PT_CAN_msg0x782.id = 0x782;
    PT_CAN_msg0x782.buf[0] = lsbA8; // 8
    PT_CAN_msg0x782.buf[1] = msbA8; // 8
    PT_CAN_msg0x782.buf[2] = lsbA9; // 9
    PT_CAN_msg0x782.buf[3] = msbA9; // 9
    PT_CAN_msg0x782.buf[4] = lsbpitch; // 10
    PT_CAN_msg0x782.buf[5] = msbpitch; // 10
    PT_CAN_msg0x782.buf[6] = lsbturboRPM1; // 11
    PT_CAN_msg0x782.buf[7] = msbturboRPM1; // 11
#endif

  #ifdef Concept_KitchenSink_SIC_Separate
    CAN_message_t PT_CAN_msg0x782;
    PT_CAN_msg0x782.len = 8;
    PT_CAN_msg0x782.id = 0x782;
    PT_CAN_msg0x782.buf[0] = lsbA8; // 8
    PT_CAN_msg0x782.buf[1] = msbA8; // 8
    PT_CAN_msg0x782.buf[2] = lsbA9; // 9
    PT_CAN_msg0x782.buf[3] = msbA9; // 9
    PT_CAN_msg0x782.buf[4] = lsbpitch; // 10
    PT_CAN_msg0x782.buf[5] = msbpitch; // 10
    PT_CAN_msg0x782.buf[6] = lsbturboRPM1; // 11
    PT_CAN_msg0x782.buf[7] = msbturboRPM1; // 11
#endif



#ifdef TylerTTRS
    CAN_message_t PT_CAN_msg0x782;
    PT_CAN_msg0x782.len = 8;
    PT_CAN_msg0x782.id = 0x782;
    PT_CAN_msg0x782.buf[0] = lsbA8; // 8
    PT_CAN_msg0x782.buf[1] = msbA8; // 8
    PT_CAN_msg0x782.buf[2] = lsbA9; // 9
    PT_CAN_msg0x782.buf[3] = msbA9; // 9
    PT_CAN_msg0x782.buf[4] = lowByte(PressureA14); // 10
    PT_CAN_msg0x782.buf[5] = highByte(PressureA14); // 10
    PT_CAN_msg0x782.buf[6] = lowByte(PressureA15); // 11
    PT_CAN_msg0x782.buf[7] = highByte(PressureA15);  // 11

#endif
  #pragma endregion

  #pragma region // 0x783 - Either IMU or SIC or EGT Data

  #ifdef TFT_DAZA_SIC_NOT_Installed 
  #ifndef TylerTTRS
        CAN_message_t PT_CAN_msg0x783;
        PT_CAN_msg0x783.len = 8;
        PT_CAN_msg0x783.id = 0x783;
        PT_CAN_msg0x783.buf[0] = lowByte(EngineRPM_0xA8 * 4); // 12
        PT_CAN_msg0x783.buf[1] = highByte(EngineRPM_0xA8 * 4); // 12
        PT_CAN_msg0x783.buf[2] = lowByte(PressureA8v_Int);  // 13
        PT_CAN_msg0x783.buf[3] = highByte(PressureA8v_Int); // 13
        PT_CAN_msg0x783.buf[4] = lowByte(PressureA9v_Int); // 14
        PT_CAN_msg0x783.buf[5] = lowByte(PressureA9v_Int); // 14
        PT_CAN_msg0x783.buf[6] = lowByte(LPFP_TriggerActive); // 15
        PT_CAN_msg0x783.buf[7] = 0; // 15
  #endif
  #endif

  #ifdef TFT_KS_EGTs_Installed
        CAN_message_t PT_CAN_msg0x783;
        PT_CAN_msg0x783.len = 8;
        PT_CAN_msg0x783.id = 0x783;
        PT_CAN_msg0x783.buf[0] =  lowByte(ET1_Int); // 12
        PT_CAN_msg0x783.buf[1] = highByte(ET1_Int); // 12
        PT_CAN_msg0x783.buf[2] =  lowByte(ET2_Int); // 13
        PT_CAN_msg0x783.buf[3] = highByte(ET2_Int); // 13
        PT_CAN_msg0x783.buf[4] =  lowByte(ET3_Int); // 14
        PT_CAN_msg0x783.buf[5] = highByte(ET3_Int); // 14
        PT_CAN_msg0x783.buf[6] =  lowByte(ET4_Int); // 15
        PT_CAN_msg0x783.buf[7] = highByte(ET4_Int); // 15

  #endif

#pragma endregion

  #pragma region // 0x784 - KalmanFiltered EngRPM Debugging 

#ifdef TFT_KS_EGTs_Installed
     CAN_message_t PT_CAN_msg0x784;
    PT_CAN_msg0x784.len = 8;
    PT_CAN_msg0x784.id = 0x784;
    PT_CAN_msg0x784.buf[0] =  lowByte(ET5_Int); // 24      //16                                    
    PT_CAN_msg0x784.buf[1] = highByte(ET5_Int); // 24     //16
    PT_CAN_msg0x784.buf[2] =  lowByte(EGTavg_Int);  // 25     //17
    PT_CAN_msg0x784.buf[3] = highByte(EGTavg_Int); // 25     //17
    PT_CAN_msg0x784.buf[4] =  lowByte(EGTErrorState); // 26     //18
    PT_CAN_msg0x784.buf[5] =  lowByte(EGT_SafetyInterventionCombined);// 26     //18
    PT_CAN_msg0x784.buf[6] =  lowByte(EGT_HotSafetyIntervention); // 27     //19
    PT_CAN_msg0x784.buf[7] =  lowByte(EGT_ColdSafetyIntervention);// 27     //19

#endif

#ifndef TFT_KS_EGTs_Installed
    CAN_message_t PT_CAN_msg0x784;
    PT_CAN_msg0x784.len = 8;
    PT_CAN_msg0x784.id = 0x784;
    PT_CAN_msg0x784.buf[0] = lowByte(rpmDOT_20ms_KalmanFiltered); // 24      //16                                    
    PT_CAN_msg0x784.buf[1] = highByte(rpmDOT_20ms_KalmanFiltered); // 24     //16
    PT_CAN_msg0x784.buf[2] = lowByte(TFT_KalmanFilteredMPC_FilteredEngRPM_Int);  // 25     //17
    PT_CAN_msg0x784.buf[3] = highByte(TFT_KalmanFilteredMPC_FilteredEngRPM_Int); // 25     //17
    PT_CAN_msg0x784.buf[4] = lowByte(TFT_KalmanFilteredMPC_PredictedEngRPM_Extrapolated200ms); // 26     //18
    PT_CAN_msg0x784.buf[5] = highByte(TFT_KalmanFilteredMPC_PredictedEngRPM_Extrapolated200ms);// 26     //18
    PT_CAN_msg0x784.buf[6] = lowByte(TFT_KalmanFilteredMPC_PredictedEngRPM_Extrapolated200ms_Filtered); // 27     //19
    PT_CAN_msg0x784.buf[7] = highByte(TFT_KalmanFilteredMPC_PredictedEngRPM_Extrapolated200ms_Filtered);// 27     //19

#endif

  #pragma endregion

  #pragma region // 0x785 mux1 (msb is set to 1, so value will be 255 in log) - TFT AutoShift and LC EngineRPM Configuration Vars

    CAN_message_t PT_CAN_msg0x785_mux1;
    PT_CAN_msg0x785_mux1.len = 8;
    PT_CAN_msg0x785_mux1.id = 0x785;
    PT_CAN_msg0x785_mux1.buf[0] = lowByte(MQB_AutoShift_EngineRPM_KalmanFilteredMPC_Gain_Int); //20
    PT_CAN_msg0x785_mux1.buf[1] = 0; //20
    PT_CAN_msg0x785_mux1.buf[2] = lowByte(MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Int); //21
    PT_CAN_msg0x785_mux1.buf[3] = highByte(MQB_AutoUPshift_CurrentGear_RPMSetpoint_Trigger_Int); //21
    PT_CAN_msg0x785_mux1.buf[4] = lowByte(MQB_LaunchControl_EngineRPM_PID_Ki_Term_Int); //22
    PT_CAN_msg0x785_mux1.buf[5] = lowByte(MQB_LaunchControl_EngineRPM_PID_BaseOffset_Int); //22
    PT_CAN_msg0x785_mux1.buf[6] = lowByte(rpmDOT_20ms); //23
    PT_CAN_msg0x785_mux1.buf[7] = highByte(rpmDOT_20ms); //23

    #ifdef KitchenSink_LevelSensors_via_DCC
        CAN_message_t PT_CAN_msg0x785_mux2;
        PT_CAN_msg0x785_mux2.len = 8;
        PT_CAN_msg0x785_mux2.id = 0x785;
        PT_CAN_msg0x785_mux2.buf[0] = lowByte(UnsignedLevelSensor_withOffset_FL); //20
        PT_CAN_msg0x785_mux2.buf[1] = highByte(UnsignedLevelSensor_withOffset_FL); //20
        PT_CAN_msg0x785_mux2.buf[2] = lsbUnsignedLevelSensor_withOffset_FR; //21
        PT_CAN_msg0x785_mux2.buf[3] = msbUnsignedLevelSensor_withOffset_FR; //21
        PT_CAN_msg0x785_mux2.buf[4] = lsbUnsignedLevelSensor_withOffset_RL; //22
        PT_CAN_msg0x785_mux2.buf[5] = msbUnsignedLevelSensor_withOffset_RL; //22
        PT_CAN_msg0x785_mux2.buf[6] = lsbUnsignedLevelSensor_withOffset_RR; //23
        PT_CAN_msg0x785_mux2.buf[7] = msbUnsignedLevelSensor_withOffset_RR; //23

    #endif

  

  #pragma endregion

  #pragma region // 0x786mux - (DQ500 DAQ and OEM Haldex Data), (Suspension Level Sensors), (EGTs)



  #ifdef AWDModule_OEMGen5Haldex
    CAN_message_t PT_CAN_msg0x786_mux1; //  DQ500 and Haldex DAQ - Pressure Actuals
    PT_CAN_msg0x786_mux1.len = 8;
    PT_CAN_msg0x786_mux1.id = 0x786;
    PT_CAN_msg0x786_mux1.buf[0] = 1; // 24
    PT_CAN_msg0x786_mux1.buf[1] = 0; // 24
    PT_CAN_msg0x786_mux1.buf[2] = lsbHaldexPWMSigInt; // 25
    PT_CAN_msg0x786_mux1.buf[3] = msbHaldexPWMSigInt; // 25
    PT_CAN_msg0x786_mux1.buf[4] = lowByte(PT_CAN_TCU_OLD_Clutch1_PressureActual_Int); // 26
    PT_CAN_msg0x786_mux1.buf[5] = highByte(PT_CAN_TCU_OLD_Clutch1_PressureActual_Int); // 26
    PT_CAN_msg0x786_mux1.buf[6] = lowByte(PT_CAN_TCU_OLD_Clutch2_PressureActual_Int); // 27
    PT_CAN_msg0x786_mux1.buf[7] = highByte(PT_CAN_TCU_OLD_Clutch2_PressureActual_Int); // 27


  #endif

  #ifdef AWDModule_SyvecsAWD
    CAN_message_t PT_CAN_msg0x786_mux1; //  DQ500 and Haldex DAQ - Pressure Actuals
    PT_CAN_msg0x786_mux1.len = 8;
    PT_CAN_msg0x786_mux1.id = 0x786;
    PT_CAN_msg0x786_mux1.buf[0] = 1; // 24
    PT_CAN_msg0x786_mux1.buf[1] = 0; // 24
    PT_CAN_msg0x786_mux1.buf[2] = lowByte(SyvecsAWD_AWDBase); // 25
    PT_CAN_msg0x786_mux1.buf[3] = highByte(SyvecsAWD_AWDBase); // 25
    PT_CAN_msg0x786_mux1.buf[4] = lowByte(PT_CAN_TCU_OLD_Clutch1_PressureActual_Int); // 26
    PT_CAN_msg0x786_mux1.buf[5] = highByte(PT_CAN_TCU_OLD_Clutch1_PressureActual_Int); // 26
    PT_CAN_msg0x786_mux1.buf[6] = lowByte(PT_CAN_TCU_OLD_Clutch2_PressureActual_Int); // 27
    PT_CAN_msg0x786_mux1.buf[7] = highByte(PT_CAN_TCU_OLD_Clutch2_PressureActual_Int); // 27

    CAN_message_t PT_CAN_msg0x786_mux2;
    PT_CAN_msg0x786_mux2.len = 8;
    PT_CAN_msg0x786_mux2.id = 0x786;
    PT_CAN_msg0x786_mux2.buf[0] = 2;
    PT_CAN_msg0x786_mux2.buf[1] = lowByte(SyvecsAWD_AWDPot); // 24
    PT_CAN_msg0x786_mux2.buf[2] = lowByte(SyvecsAWD_AWDBase); // 25
    PT_CAN_msg0x786_mux2.buf[3] = highByte(SyvecsAWD_AWDBase); // 25
    PT_CAN_msg0x786_mux2.buf[4] = lowByte(SyvecsAWD_AWDFinal);  //26
    PT_CAN_msg0x786_mux2.buf[5] = highByte(SyvecsAWD_AWDFinal); //26
    PT_CAN_msg0x786_mux2.buf[6] = lowByte(SyvecsAWD_CenterSlipAdder);  //27
    PT_CAN_msg0x786_mux2.buf[7] = highByte(SyvecsAWD_CenterSlipAdder); //27


  #endif


  #pragma endregion

  #pragma region // 0x787 - Wheel Speeds relayed (sniffed from 0xB2)

    WhlSpdFL_Char = WhlSpdFL_Float * 100;
    WhlSpdFR_Char = WhlSpdFR_Float * 100;
    WhlSpdRL_Char = WhlSpdRL_Float * 100;
    WhlSpdRR_Char = WhlSpdRR_Float * 100;
    WhlSpdAvg_Char = WhlSpdAvg_Float * 100;


    int WhlSpdFL_Char_Int = WhlSpdFL_Float * 100;
    int WhlSpdFR_Char_Int = WhlSpdFR_Float * 100;
    int WhlSpdRL_Char_Int = WhlSpdRL_Float * 100;
    int WhlSpdRR_Char_Int = WhlSpdRR_Float * 100;
    int WhlSpdAvg_Char_Int = WhlSpdAvg_Float * 100;

    CAN_message_t PT_CAN_msg0x787_mux1;
    PT_CAN_msg0x787_mux1.len = 8;
    PT_CAN_msg0x787_mux1.id = 0x787;
    PT_CAN_msg0x787_mux1.buf[0] =  lowByte(WhlSpdRL_Char_Int); // 28 CANext28 will be less than 512
    PT_CAN_msg0x787_mux1.buf[1] = highByte(WhlSpdRL_Char_Int); // 28
    PT_CAN_msg0x787_mux1.buf[2] =  lowByte(WhlSpdRR_Char_Int); // 29 
    PT_CAN_msg0x787_mux1.buf[3] = highByte(WhlSpdRR_Char_Int); // 29
    PT_CAN_msg0x787_mux1.buf[4] =  lowByte(WhlSpdFL_Char_Int); // 30 
    PT_CAN_msg0x787_mux1.buf[5] = highByte(WhlSpdFL_Char_Int); // 30
    PT_CAN_msg0x787_mux1.buf[6] =  lowByte(WhlSpdFR_Char_Int); // 31 
    PT_CAN_msg0x787_mux1.buf[7] = highByte(WhlSpdFR_Char_Int); // 31

    #ifdef TFT_KS_EGTs_Installed

    CAN_message_t PT_CAN_msg0x787_mux2;
    PT_CAN_msg0x787_mux2.len = 8;
    PT_CAN_msg0x787_mux2.id = 0x787;
    PT_CAN_msg0x787_mux2.buf[0] = 0; // 28 // CANext28 will be 512
    PT_CAN_msg0x787_mux2.buf[1] = 250; // 28 // CANext28 will be 512
    PT_CAN_msg0x787_mux2.buf[2] = lsbEGT2; // 29
    PT_CAN_msg0x787_mux2.buf[3] = msbEGT2; // 29
    PT_CAN_msg0x787_mux2.buf[4] = lsbEGT3; // 30
    PT_CAN_msg0x787_mux2.buf[5] = msbEGT3; // 30
    PT_CAN_msg0x787_mux2.buf[6] = lsbEGT4; // 31
    PT_CAN_msg0x787_mux2.buf[7] = msbEGT4; // 31

    CAN_message_t PT_CAN_msg0x787_mux3;
    PT_CAN_msg0x787_mux3.len = 8;
    PT_CAN_msg0x787_mux3.id = 0x787;
    PT_CAN_msg0x787_mux3.buf[0] = 0; // 28 // CANext28 will be 1024
    PT_CAN_msg0x787_mux3.buf[1] = 251; // 28 // CANext28 will be 1024
    PT_CAN_msg0x787_mux3.buf[2] = lsbEGT5; // 29
    PT_CAN_msg0x787_mux3.buf[3] = msbEGT5; // 29
    PT_CAN_msg0x787_mux3.buf[4] = lsbEGT6; // 30
    PT_CAN_msg0x787_mux3.buf[5] = msbEGT6; // 30
    PT_CAN_msg0x787_mux3.buf[6] = lsbEGT7; // 31
    PT_CAN_msg0x787_mux3.buf[7] = msbEGT7; // 31


    #endif


    // CAN_message_t PT_CAN_msg0x787_mux1;
    // PT_CAN_msg0x787_mux1.len = 8;
    // PT_CAN_msg0x787_mux1.id = 0x787;
    // PT_CAN_msg0x787_mux1.buf[0] =  lowByte(DQ500_CurrentGear_0x394); // 28 
    // PT_CAN_msg0x787_mux1.buf[1] = highByte(DQ500_CurrentGear_0x394); // 28
    // PT_CAN_msg0x787_mux1.buf[2] =  lowByte(DQ500_ClutchLockStatus); // 29 
    // PT_CAN_msg0x787_mux1.buf[3] = highByte(DQ500_ClutchLockStatus); // 29
    // PT_CAN_msg0x787_mux1.buf[4] =  lowByte(DQ500_TargetGear2_0x394); // 30 
    // PT_CAN_msg0x787_mux1.buf[5] = highByte(DQ500_TargetGear2_0x394); // 30
    // PT_CAN_msg0x787_mux1.buf[6] =  lowByte(DQ500_GoalGear0xAD); // 31 
    // PT_CAN_msg0x787_mux1.buf[7] = highByte(DQ500_GoalGear0xAD); // 31

  #pragma endregion                                                     


  #pragma region // 0x788mux -  
  //   // CAN_message_t PT_CAN_msg0x788_mux1;
  //   // PT_CAN_msg0x788_mux1.len = 8;
  //   // PT_CAN_msg0x788_mux1.id = 0x747;
  //   // PT_CAN_msg0x788_mux1.buf[0] = 11;
  //   // PT_CAN_msg0x788_mux1.buf[1] = 22;
  //   // PT_CAN_msg0x788_mux1.buf[2] = 33;
  //   // PT_CAN_msg0x788_mux1.buf[3] = 44;
  //   // PT_CAN_msg0x788_mux1.buf[4] = 55;
  //   // PT_CAN_msg0x788_mux1.buf[5] = 66;
  //   // PT_CAN_msg0x788_mux1.buf[6] = 77;
  //   // PT_CAN_msg0x788_mux1.buf[7] = 88;
  #pragma endregion


  #pragma region //  0x794 MUX - sending basic data to OBD_CAN to be recieved by DS1 for DS2 datalogging

    // CAN_message_t PT_CAN_msg0x788_mux1;
    // PT_CAN_msg0x788_mux1.len = 8;
    // PT_CAN_msg0x788_mux1.id = 0x747;
    // PT_CAN_msg0x788_mux1.buf[0] = 11;
    // PT_CAN_msg0x788_mux1.buf[1] = 22;
    // PT_CAN_msg0x788_mux1.buf[2] = 33;
    // PT_CAN_msg0x788_mux1.buf[3] = 44;
    // PT_CAN_msg0x788_mux1.buf[4] = 55;
    // PT_CAN_msg0x788_mux1.buf[5] = 66;
    // PT_CAN_msg0x788_mux1.buf[6] = 77;
    // PT_CAN_msg0x788_mux1.buf[7] = 88;



   
    CAN_message_t PT_CAN_msg0x794_mux1;
    PT_CAN_msg0x794_mux1.len = 8;
    PT_CAN_msg0x794_mux1.id = 0x794;
    PT_CAN_msg0x794_mux1.buf[0] = 11;
    PT_CAN_msg0x794_mux1.buf[1] = 22;
    PT_CAN_msg0x794_mux1.buf[2] = 33;
    PT_CAN_msg0x794_mux1.buf[3] = 44;
    PT_CAN_msg0x794_mux1.buf[4] = 55;
    PT_CAN_msg0x794_mux1.buf[5] = 66;
    PT_CAN_msg0x794_mux1.buf[6] = 77;
    PT_CAN_msg0x794_mux1.buf[7] = 88;

  #pragma endregion



  #pragma endregion

#pragma region // CAN Message Transmits to DS1 Frames 0x780 to 0x787

 if(IgnitionStatusTimer_TimeSinceLastMessage < 1000) {

 
  // if (PT_CAN_canTxInterval_0x107 > 5000 && PT_CAN_canTxInterval_0x147 > 5000) {
    // digitalWrite(LED_PIN_BoardPower, LOW);

    if (PT_CAN_canTxInterval_0x780 > 60) {
        
        TFTCAN3.write(PT_CAN_msg0x780); // CAN Frame for IMU Yaw, Pitch, Roll
        PT_CAN_canTxInterval_0x780 = 0;
      // Serial.println("");
      // Serial.print("CAN0x780 Sent... ");
      // Serial.println("");

    }

    if (PT_CAN_canTxInterval_0x781 > 40) { // Multiplexed CAN frames for general TFT Function Configurations

      TFTCAN3.write(PT_CAN_msg0x781);  // CAN Frame for TFT Configuration States
      PT_CAN_canTxInterval_0x781 = 0;
      // Serial.println("");
      // Serial.print("CAN0x781 Sent... ");
      // Serial.println("");

    }

   /////////////////////////////////////////      PT_CAN_msg0x781_muxXXX - Multiplexed CAN frames for general TFT Function Configurations

    // if (PT_CAN_canTxInterval_0x781 == 50) { // Multiplexed CAN frames for general TFT Function Configurations

    //   TFTCAN3.write(PT_CAN_msg0x781_mux1);  // CAN Frame for TFT Configuration States
    //   PT_CAN_canTxInterval_0x781 = 51;
    //   // Serial.println("");
    //   // Serial.print("CAN0x781_mux1 Sent... ");
    //   // Serial.println("");

    // }

    // if (PT_CAN_canTxInterval_0x781 == 100) { // Multiplexed CAN frames for general TFT Function Configurations

    //   TFTCAN3.write(PT_CAN_msg0x781_mux2);  // CAN Frame for TFT Configuration States
    //   PT_CAN_canTxInterval_0x781 = 101;
    //   // Serial.println("");
    //   // Serial.print("CAN0x781_mux2 Sent... ");
    //   // Serial.println("");

    // }

    // if (PT_CAN_canTxInterval_0x781 == 150) { // Multiplexed CAN frames for general TFT Function Configurations

    //   TFTCAN3.write(PT_CAN_msg0x781_mux3);  // CAN Frame for TFT Configuration States
    //   PT_CAN_canTxInterval_0x781 = 151;
    //   // Serial.println("");
    //   // Serial.print("CAN0x781_mux3 Sent... ");
    //   // Serial.println("");

    // }

    // if (PT_CAN_canTxInterval_0x781 == 200) { // Multiplexed CAN frames for general TFT Function Configurations

    //   TFTCAN3.write(PT_CAN_msg0x781_mux4);  // CAN Frame for TFT Configuration States
    //   PT_CAN_canTxInterval_0x781 = 201;
    //   // Serial.println("");
    //   // Serial.print("CAN0x781_mux4 Sent... ");
    //   // Serial.println("");

    // }

    // if (PT_CAN_canTxInterval_0x781 == 250) { // Multiplexed CAN frames for general TFT Function Configurations

    //   TFTCAN3.write(PT_CAN_msg0x781_mux5);  // CAN Frame for TFT Configuration States
    //   PT_CAN_canTxInterval_0x781 = 0;
    //   // Serial.println("");
    //   // Serial.print("CAN0x781_mux5 Sent... ");
    //   // Serial.println("");

    // }


    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (PT_CAN_canTxInterval_0x782 > 20) {

      TFTCAN3.write(PT_CAN_msg0x782); // CAN Frame for IMAP and EMAP Pressure Sensors
      PT_CAN_canTxInterval_0x782 = 0;
    }

    #ifdef TFT_DAZA_SIC_v1x 
      if (PT_CAN_canTxInterval_0x783 >= 60) { // this is in millis

            TFTCAN3.write(PT_CAN_msg0x783); //  CAN Frame for Yaw, Pitch, Roll
            PT_CAN_canTxInterval_0x783 = 0;

          }

      #endif


    #ifdef TylerTTRS 
      if (PT_CAN_canTxInterval_0x783 >= 40) { // this is in millis

            TFTCAN3.write(PT_CAN_msg0x783); //  CAN Frame for Yaw, Pitch, Roll
            PT_CAN_canTxInterval_0x783 = 0;

          }

      #endif

    #ifdef DOC_BasicDAZA_SecondaryLPFP_Trigger 
      if (PT_CAN_canTxInterval_0x783 > 50) { // this is in millis

            TFTCAN3.write(PT_CAN_msg0x783); //  CAN Frame for Yaw, Pitch, Roll
            PT_CAN_canTxInterval_0x783 = 0;

          }

      #endif



    if (PT_CAN_canTxInterval_0x784 > 40) { // TFT EngineRPM DOT and KalmanFilteredPredictions

          TFTCAN3.write(PT_CAN_msg0x784); //  CAN Frame for A0, A1, A2, A3
          PT_CAN_canTxInterval_0x784 = 0;

        }


      #ifndef KitchenSink_LevelSensors_via_DCC
      
    if (PT_CAN_canTxInterval_0x785 > 50) { // TFT AutoShift and LC EngineRPM Configuration Variables

      TFTCAN3.write(PT_CAN_msg0x785_mux1); //  CAN Frame for A0, A1, A2, A3
      PT_CAN_canTxInterval_0x785 = 0;
      // Serial.print("PT_CAN_canTxInterval_0x785 sent...");
      // Serial.println();
    }
          #endif


    #ifdef KitchenSink_LevelSensors_via_DCC

    if (PT_CAN_canTxInterval_0x785 > 30 && PT_CAN_canTxInterval_0x785_mux2 > 30 ) { // TFT AutoShift and Susp Level Sensors

        TFTCAN3.write(PT_CAN_msg0x785_mux1); // 
        PT_CAN_canTxInterval_0x785 = 0;
        PT_CAN_canTxInterval_0x785_mux2 = 30;
      }
      
    if (PT_CAN_canTxInterval_0x785_mux2 > 59) { // Suspension Level Sensors

        TFTCAN3.write(PT_CAN_msg0x785_mux2); // 
        PT_CAN_canTxInterval_0x785 = 0;
        PT_CAN_canTxInterval_0x785_mux2 = 0;
      }
      


    #endif




      }


    #ifndef AWDModule_SyvecsAWD

    if (PT_CAN_canTxInterval_0x786_mux1 > 50) { // DQ500 DAQ Pressures and OEM Haldex PWM

        TFTCAN3.write(PT_CAN_msg0x786_mux1); // 
        PT_CAN_canTxInterval_0x786_mux1 = 0;
      }
      
    #endif  



    #ifdef AWDModule_SyvecsAWD

    if (PT_CAN_canTxInterval_0x786_mux1 > 60 && PT_CAN_canTxInterval_0x786_mux2 > 60 ) { // DQ500 DAQ Pressures and Syvecs AWD Info and EGT

        TFTCAN3.write(PT_CAN_msg0x786_mux1); // 
        PT_CAN_canTxInterval_0x786_mux1 = 0;
        PT_CAN_canTxInterval_0x786_mux2 = 60;
      }
      
    if (PT_CAN_canTxInterval_0x786_mux2 > 119) { // Syvecs AWD

        TFTCAN3.write(PT_CAN_msg0x786_mux2); // 
        PT_CAN_canTxInterval_0x786_mux1 = 0;
        PT_CAN_canTxInterval_0x786_mux2 = 0;
      }
      


    #endif




    // if (PT_CAN_canTxInterval_0x786_mux1 > 50) { // DQ500 DAQ Pressures and OEM Haldex PWM

    //     TFTCAN3.write(PT_CAN_msg0x786_mux1); // 

    //     PT_CAN_canTxInterval_0x786_mux1 = 0;


    //   }
      
    #ifndef TFT_KS_EGTs_Installed
      if (PT_CAN_canTxInterval_0x787_mux1 > 50) { // Wheel Speeds relayed (sniffed from 0xB2)

          TFTCAN3.write(PT_CAN_msg0x787_mux1); //  

          PT_CAN_canTxInterval_0x787_mux1 = 0;

        }
    #endif
    
    
    #ifdef TFT_KS_EGTs_Installed

    if (PT_CAN_canTxInterval_0x787_mux1 > 29 && PT_CAN_canTxInterval_0x787_mux2 > 59 && PT_CAN_canTxInterval_0x787_mux3 > 89 ) { // WheelSpeeds and EGTs

        TFTCAN3.write(PT_CAN_msg0x787_mux1); // Mux1 - Wheel Speeds
        PT_CAN_canTxInterval_0x787_mux1 = 0;
        PT_CAN_canTxInterval_0x787_mux2 = 30;
        PT_CAN_canTxInterval_0x787_mux3 = 30;
        // Serial.println("PT_CAN_canTxInterval_0x787_mux1 sent");

      }
      
    if (PT_CAN_canTxInterval_0x787_mux2 > 59) { //  Mux2 - EGT Set1

        TFTCAN3.write(PT_CAN_msg0x787_mux2); // 
        PT_CAN_canTxInterval_0x787_mux1 = 0;
        PT_CAN_canTxInterval_0x787_mux2 = 0;
        PT_CAN_canTxInterval_0x787_mux3 = 60;
        // Serial.println("PT_CAN_canTxInterval_0x787_mux2 sent");

      }

    if (PT_CAN_canTxInterval_0x787_mux3 > 89) { // Mux3 - EGT Set2

        TFTCAN3.write(PT_CAN_msg0x787_mux3); // 
        PT_CAN_canTxInterval_0x787_mux1 = 0;
        PT_CAN_canTxInterval_0x787_mux2 = 30;
        PT_CAN_canTxInterval_0x787_mux3 = 60;
        // Serial.println("PT_CAN_canTxInterval_0x787_mux3 sent");

      }

    #endif





    if (PT_CAN_canTxInterval_0x788_mux1 > 80) { // this is in millis

        // TFTCAN3.write(PT_CAN_msg0x788_mux1); //  CAN Frame for A0, A1, A2, A3
        PT_CAN_canTxInterval_0x788_mux1 = 0;
      // Serial.println("");
      // Serial.print("CAN0x788 Sent... ");
      // Serial.println("");

      }



#pragma endregion

}

void do_TFTCAN_PT_CAN_Transmits_AIM(){


#pragma region // CAN Frame Creations and Transmits - From TFT Haldex Controller to be consumed by AIM MXG Dash

        // Serial.println("");
        // Serial.print("PT_CAN_msg0x786_forAIM Sent... ");
        // Serial.println("");

  #pragma region // Create 0x786, 0x787 and 0x789 - TFT Standalone Haldex Controller Data


        CAN_message_t PT_CAN_msg0x786_forAIM;
        PT_CAN_msg0x786_forAIM.len = 8;
        PT_CAN_msg0x786_forAIM.id = 0x786;
        PT_CAN_msg0x786_forAIM.buf[0] =  2;
        PT_CAN_msg0x786_forAIM.buf[1] =  1;  
        PT_CAN_msg0x786_forAIM.buf[2] =  lowByte(HaldexPWMOutput_Final_Int);   
        PT_CAN_msg0x786_forAIM.buf[3] =  lowByte(HaldexPWMOutput_FrontSlip_Adder_Int);    
        PT_CAN_msg0x786_forAIM.buf[4] =  lowByte(HaldexPWMOutput_RearSlip_Adder_Int);  
        PT_CAN_msg0x786_forAIM.buf[5] =  lowByte(HaldexPWMOutput_CenterSlip_Adder_Int);  
        PT_CAN_msg0x786_forAIM.buf[6] =  lowByte(HaldexPWMOutput_BrakePressureAdder_Int);
        PT_CAN_msg0x786_forAIM.buf[7] =  lowByte(HaldexPWMOutput_CoastMinDC_Adder_Int);



        CAN_message_t PT_CAN_msg0x789_forAIM; // Trim Pot Positions
        PT_CAN_msg0x789_forAIM.len = 8;
        PT_CAN_msg0x789_forAIM.id = 0x789;
        PT_CAN_msg0x789_forAIM.buf[0] =  1;    
        PT_CAN_msg0x789_forAIM.buf[1] =  1;
        PT_CAN_msg0x789_forAIM.buf[2] =  lowByte(HaldexPWMOutput_TrimPot_AWD_BrakePressureAdder_Multiplier_Position_Int);
        PT_CAN_msg0x789_forAIM.buf[3] =  lowByte(HaldexPWMOutput_TrimPot_AWD_CoastMinDC_Position_Int);
        PT_CAN_msg0x789_forAIM.buf[4] =  lowByte(HaldexPWMOutput_TrimPot_AWD_SteeringAngleMultiplier_Position_Int);     
        PT_CAN_msg0x789_forAIM.buf[5] =  lowByte(HaldexPWMOutput_TrimPot_AWD_MaxDC_Position_Int);  
        PT_CAN_msg0x789_forAIM.buf[6] =  lowByte(HaldexPWMOutput_TrimPot_AWD_Future1_Position_Int); 
        PT_CAN_msg0x789_forAIM.buf[7] =  lowByte(HaldexPWMOutput_TrimPot_AWD_Future2_Position_Int); 
        

        CAN_message_t PT_CAN_msg0x790_forAIM;
        PT_CAN_msg0x790_forAIM.len = 8;
        PT_CAN_msg0x790_forAIM.id = 0x790;
        PT_CAN_msg0x790_forAIM.buf[0] = lowByte(HaldexPWMOutput_TrimPot_AWD_AccelPedalGain_Multiplier_Int);   
        PT_CAN_msg0x790_forAIM.buf[1] = lowByte(HaldexPWMOutput_TrimPot_AWD_GlobalMultiplier_Int);   
        PT_CAN_msg0x790_forAIM.buf[2] = lowByte(HaldexPWMOutput_TrimPot_AWD_BrakePressureAdder_Multiplier_Int);   
        PT_CAN_msg0x790_forAIM.buf[3] = lowByte(HaldexPWMOutput_TrimPot_AWD_CoastMinDC_Adder_Int);   
        PT_CAN_msg0x790_forAIM.buf[4] = lowByte(HaldexPWMOutput_TrimPot_AWD_SteeringAngleMultiplier_Int);   
        PT_CAN_msg0x790_forAIM.buf[5] = lowByte(HaldexPWMOutput_TrimPot_AWD_MaxDC_Int);   
        PT_CAN_msg0x790_forAIM.buf[6] = lsbInternalCPUTemp;   
        PT_CAN_msg0x790_forAIM.buf[7] = lowByte(HaldexPWMOutput_Final_Int);   


  #pragma endregion

  #pragma region // CAN Message Transmits of Frames 0x786, 0x787, 0x789 to be consumed by AIM Dash


      if (PT_CAN_canTxInterval_0x786_forAIM > 100) { // this is in millis

          TFTCAN3.write(PT_CAN_msg0x786_forAIM); //  CAN Frame for A0, A1, A2, A3
          PT_CAN_canTxInterval_0x786_forAIM = 0;
        // Serial.println("");
        // Serial.print("PT_CAN_msg0x786_forAIM Sent... ");
        // Serial.println("");

        }

      if (PT_CAN_canTxInterval_0x789_forAIM > 500) { // this is in millis

          TFTCAN3.write(PT_CAN_msg0x789_forAIM); //  CAN Frame for A4, A5, A8, A9
          PT_CAN_canTxInterval_0x789_forAIM = 0;
        // Serial.println("");
        // Serial.print("PT_CAN_msg0x789_forAIM Sent... ");
        // Serial.println("");

        }

      if (PT_CAN_canTxInterval_0x790_forAIM > 500) { // this is in millis

          TFTCAN3.write(PT_CAN_msg0x790_forAIM); //  CAN Frame for A4, A5, A8, A9
          PT_CAN_canTxInterval_0x790_forAIM = 0;
        // Serial.println("");
        // Serial.print("PT_CAN_msg0x790_forAIM Sent... ");
        // Serial.println("");

        }



  #pragma endregion


#pragma endregion



}



void do_TFTCAN_PT_CAN_DQ500_DAQ_Loop_PressureActualsOnly() {

  // TFTCAN3.events();
  if(loopDelayPT_CAN_KeepAwake_0x700 > 1000) {
    // canTx_PT_CAN_GeneralKeepAwake_0x700_2_3E_80();
    loopDelayPT_CAN_KeepAwake_0x700 = 0;
  }


#pragma region // CAN Input processing - OLD TCU Set1 - PressureActuals Only



#pragma region  // Send canTx Functions for OLD TCU Set 1 Data Requests - Continuous


    #pragma region // Send CAN Tx OBD requests - Service22 - 0x7E1 - Old TCU Set 1 - Clutch Pressure Actuals Only via PT_CAN

    // if (PT_CAN_DQ500_DAQ_OLD_Set1_SequenceComplete == true )  // TCU - Set 1 - Clutch Pressure Actuals
    if ((TimeSinceSTATFC1_PT_CAN_OLD_TCU_ReqSet1 > 30 && PT_CAN_DQ500_DAQ_OLD_Set1_SequenceComplete == true) && PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22 == 0)  // TCU - Set 1 - Clutch Pressure Actuals
    // if ( PT_CAN_DQ500_DAQ_OLD_Set1_SequenceComplete == true )  // TCU - Set 1 - Clutch Pressure Actuals
    // if (TimeSinceSTATFC1_PT_CAN_OLD_TCU_ReqSet1 > 40  )  // TCU - Set 1 - Clutch Pressure Actuals
        {
            PT_CAN_DQ500_DAQ_OLD_Set1_SequenceComplete = false;

          // if (PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22 == 0 && DS1_ECU_LoggingConfigActive > 8000){
              //     Serial.print("PT_CAN TCUSet1Msg1Pre-Sent... ");  Serial.println(" "); 
              // Serial.print("PT_CAN TCU Set1 First Response Recieved...: "); Serial.print("TimeSinceFirstResponseRecieved:  ");  Serial.print(TimeSinceFirstResponseRecieved_TCU_Set1); Serial.println("");

                  canTx_PT_CAN_22_TCU_OLD_Set1_PressureActualsOnly(); // Query data from CAN BUS via OBD
      // Serial.print("PT_CAN_TCUSet1Msg1 Sent...  ");  Serial.println(" "); 

                  // Serial.print("PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22  ");  Serial.print(PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22);  Serial.print(" "); 
                  // Serial.print("loopDelayTCU_Req1  ");  Serial.print(loopDelayPT_CAN_TCU_ReqSet1_ClutchPressureActuals);  Serial.print(" "); 
                  // Serial.print(""    ");
                  //  Serial.print("PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22:  "); Serial.print(PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22); Serial.println(" "); 
                  // int PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22 = PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22;

                // }
                  //  loopDelayPT_CAN_TCU_ReqSet1_ClutchPressureActuals = 0;
                  // Serial.print("TCUSet1Msg1 Sent  ");  Serial.print(" "); 
                  // Serial.print("StatFC:  ");  Serial.print(PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22);  Serial.println(" "); 

      }


    if (PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22 == 1 && DS1_ECU_LoggingConfigActive > 8000 && PT_CAN_DQ500_DAQ_OLD_Set1_SequenceComplete == false && TimeSinceFirstResponseRecieved_TCU_Set1 > 10 ){

                // delay(5);
                //  Serial.print(" ");  
              // Serial.print("PT_CAN TCU Set1 First Response Recieved...: "); Serial.print("TimeSinceFirstResponseRecieved:  ");  Serial.print(TimeSinceFirstResponseRecieved_TCU_Set1); Serial.println("");

                // Serial.print("StatFCbeforeStatFC1sent:  ");  Serial.print("\t"); Serial.print("STAT_FC_22:  ");  Serial.print(PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22);  Serial.println(""); 
// 
                canTx_PT_CAN_22_TCU_OLD_Set1_PressureActualsOnly_FlowControl();
              // Serial.print("PT_CAN TCU Set1 First Response Recieved...: "); Serial.print("TimeSinceFirstResponseRecieved:  ");  Serial.print(TimeSinceFirstResponseRecieved_TCU_Set1); Serial.println("");

                // PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22 = 2;
                // Serial.print("TCUSet1_FCMsg instruction Sent ");  Serial.print(" "); 
                // Serial.print("StatFCafterStatFC1sent:  ");  Serial.print(PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22);  Serial.println(" "); 
                // Serial.print("TimeSinceTCU_FC1  ");  Serial.print(TimeSinceSTATFC1_PT_CAN_OLD_TCU_ReqSet1);  Serial.print(" "); 

                //  Serial.println(" ");
                //  Serial.println("canTx_PT_CAN_22_TCU_OLD_Set1_PressureActualsOnly_FlowControl Sent");  
                //  Serial.println("\t"); 
                //  int PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22 = PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22;
                //  Serial.print("PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22: ");
                //  Serial.println(PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22);

                // loopDelayPT_CAN_TCU_ReqSet1_ClutchPressureActuals = 0;
                // TimeSinceSTATFC1_PT_CAN_OLD_TCU_ReqSet1 = 0;
// return;
            }
            
    // if (TimeSinceSTATFC1_PT_CAN_OLD_TCU_ReqSet1 > 5000 && DS1_ECU_LoggingConfigActive > 8000 && PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22 > 0) {

    //                   Serial.print("Sending PT_CAN_TCUSet1Msg1 due to Timeout  ");  Serial.println(" "); 

    //             PT_CAN_DQ500_DAQ_OLD_Set1_SequenceComplete = true;
    //             PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22 = 0;
    //             PT_CAN_TCU_0x7E9_PressureActuals_CF_Num = 0;
    //             PT_CAN_f_sid22_TCU_PressureActuals_multi = 0;   
    //             canTx_PT_CAN_22_TCU_OLD_Set1_PressureActualsOnly(); // Query data from CAN BUS via OBD
    //             TimeSinceSTATFC1_PT_CAN_OLD_TCU_ReqSet1 = 0;

    //             }




#pragma endregion


    // }
  // }




#pragma endregion



  #pragma region // Process 0x7E9 - OLD TCU Requests Sets (Clutch Pressure Actuals only)
  // TCU Variables Identifiers
  // 10_2B - Clutch1 Friction Lining Temp
  // 10_2C - Clutch2 Friction Lining Temp
  // 10_2A - Clutch Center Plate temp
  // 21_4 - DQ500 Oil Temp
  // 38_42 - DQ500 Oil Pressure Target
  // 38_1D - DQ500 Oil Pressure Actual
  // 38_2E - Clutch 1 Torque Target
  // 38_2F - Clutch 1 Torque Actual
  // 38_38 - Clutch 2 Torque Target
  // 38_39 - Clutch 2 Torque Actual
  // 38_EC - Clutch 1 Pressure Target
  // 38_EA - Clutch 1 Pressure Actual
  // 38_ED - Clutch 2 Pressure Target
  // 38_EB - Clutch 2 Pressure Actual


      if (PT_CAN_msg0x7E9_buf0_Raw == 0x10 && PT_CAN_msg0x7E9_buf1_Raw == 0x0D && PT_CAN_msg0x7E9_buf4_Raw == 0xEB && PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22 == 0){    ////////// Service22 Positive Response message and First Frame.
              PT_CAN_TCU_0x7E9_PressureActuals_CF_Max = 1;
              PT_CAN_f_sid22_TCU_PressureActuals_multi = 1;   // 
              TimeSinceFirstResponseRecieved_TCU_Set1 = 0;
              // Serial.print("PT_CAN TCU Set1 First Response Recieved...: "); Serial.print("TimeSinceFirstResponseRecieved:  ");  Serial.print(TimeSinceFirstResponseRecieved_TCU_Set1); Serial.println("");
              // Serial.print("STAT_FC_22:  "); Serial.print(PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22); Serial.println(" ");

              PT_CAN_DQ500_DAQ_OLD_Set1_SequenceComplete = false;
                
              // if(PT_CAN_msg0x7E9_buf2_Raw == 0x62 && TimeSinceFirstResponseRecieved_TCU_Set1 > 6){   // 
              if(PT_CAN_msg0x7E9_buf2_Raw == 0x62){   // 
                  // ignore msg0x7E9.buf[4] -  buffer is always 0x01 /Number of data item.
              // Serial.print("PT_CAN TCU Set1 message buf2raw equaled 0x62...: "); Serial.print("\t"); 
              // Serial.print("STAT_FC_22:  "); Serial.print(PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22); Serial.println(" ");
              // Serial.print("PT_CAN TCU Set1 First Response Recieved...: "); Serial.print("TimeSinceFirstResponseRecieved:  ");  Serial.print(TimeSinceFirstResponseRecieved_TCU_Set1); Serial.println("");

                  PT_CAN_msg0x7E9_Clutch2_PressureActual_LSB = PT_CAN_msg0x7E9_buf5_Raw;
                  PT_CAN_msg0x7E9_Clutch2_PressureActual_MSB = PT_CAN_msg0x7E9_buf6_Raw;  // Clutch2 Actual is identifier EB

                  PT_CAN_msg0x7E9_Clutch2_PressureActual_Raw = (PT_CAN_msg0x7E9_Clutch2_PressureActual_LSB + (PT_CAN_msg0x7E9_Clutch2_PressureActual_MSB * 256));
                  PT_CAN_msg0x7E9_Clutch2_PressureActual_Int = PT_CAN_msg0x7E9_Clutch2_PressureActual_Raw;

                  PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22 = 1; 
                               
                  // Serial.print("STAT_FC_22 Changed to 1...: "); Serial.print("\t"); 
                  // Serial.print("STAT_FC_22:  "); Serial.print(PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22); Serial.println(" ");


                  TimeSinceSTATFC1_PT_CAN_OLD_TCU_ReqSet1 = 0;
                //  Serial.print("PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22: "); Serial.print(PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22); Serial.println(" ");
                //  Serial.print("PT_CAN_f_sid22_TCU_PressureActuals_multi: "); Serial.print(PT_CAN_f_sid22_TCU_PressureActuals_multi); Serial.println(" ");
                  // Serial.print(PT_CAN_msg0x7E9_buf0_Raw, HEX);     Serial.print(" ");
                  // Serial.print(PT_CAN_msg0x7E9_buf1_Raw, HEX);     Serial.print(" ");
                  // Serial.print(PT_CAN_msg0x7E9_buf2_Raw, HEX);     Serial.print(" ");
                  // Serial.print(PT_CAN_msg0x7E9_buf3_Raw, HEX);     Serial.print(" ");
                  // Serial.print(PT_CAN_msg0x7E9_buf4_Raw, HEX);     Serial.print(" ");
                  // Serial.print(PT_CAN_msg0x7E9_buf5_Raw, HEX);     Serial.print(" ");
                  // Serial.print(PT_CAN_msg0x7E9_buf6_Raw, HEX);     Serial.print(" ");
                  // Serial.print(PT_CAN_msg0x7E9_buf7_Raw, HEX);     Serial.print(" ");
                  // Serial.println("");

              }
            }

      else if(PT_CAN_f_sid22_TCU_PressureActuals_multi == 1 && PT_CAN_msg0x7E9_buf0_Raw > 0x20 && PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22 == 2 ){     // Means CF: Consecutive Frame and Buf[0] is sequence number
          PT_CAN_TCU_0x7E9_PressureActuals_seq_Num = PT_CAN_msg0x7E9_buf0_Raw; //
              //    Serial.println("Starting PT_CAN_TCU_OldSet1_ElseIF  ");
              // Serial.print("PT_CAN TCU Set1 First Response Recieved...: "); Serial.print("TimeSinceFirstResponseRecieved:  ");  Serial.print(TimeSinceFirstResponseRecieved_TCU_Set1); Serial.println("");
          //        Serial.print(PT_CAN_msg0x7E9_buf0_Raw, HEX);     Serial.print(" ");
          //        Serial.print(PT_CAN_msg0x7E9_buf1_Raw, HEX);     Serial.print(" ");
          //        Serial.print(PT_CAN_msg0x7E9_buf2_Raw, HEX);     Serial.print(" ");
          //        Serial.print(PT_CAN_msg0x7E9_buf3_Raw, HEX);     Serial.print(" ");
          //        Serial.print(PT_CAN_msg0x7E9_buf4_Raw, HEX);     Serial.print(" ");
          //        Serial.print(PT_CAN_msg0x7E9_buf5_Raw, HEX);     Serial.print(" ");
          //        Serial.print(PT_CAN_msg0x7E9_buf6_Raw, HEX);     Serial.print(" ");
          //        Serial.print(PT_CAN_msg0x7E9_buf7_Raw, HEX);     Serial.print(" ");
          //        Serial.println("");

              if(PT_CAN_TCU_0x7E9_PressureActuals_seq_Num == 0x21){   // Sequence Number is 1.
              // Serial.print("PT_CAN TCU Set1 First Response Recieved...: "); Serial.println("");
              // Serial.print("TimeSinceFirstResponseRecieved:  ");  Serial.print(TimeSinceFirstResponseRecieved_TCU_Set1); Serial.println("");

                  PT_CAN_msg0x7E9_Clutch1_PressureActual_LSB = PT_CAN_msg0x7E9_buf2_Raw;
                  PT_CAN_msg0x7E9_Clutch1_PressureActual_MSB = PT_CAN_msg0x7E9_buf3_Raw;  // Clutch1 Actual is identifier EA

                  PT_CAN_msg0x7E9_DQ500_OilPressureActual_LSB = PT_CAN_msg0x7E9_buf6_Raw;
                  PT_CAN_msg0x7E9_DQ500_OilPressureActual_MSB = PT_CAN_msg0x7E9_buf7_Raw;  // Clutch1 Actual is identifier EA


                  PT_CAN_msg0x7E9_Clutch1_PressureActual_Raw = (PT_CAN_msg0x7E9_Clutch1_PressureActual_LSB + (PT_CAN_msg0x7E9_Clutch1_PressureActual_MSB * 256));
                  PT_CAN_msg0x7E9_Clutch1_PressureActual_Int = PT_CAN_msg0x7E9_Clutch1_PressureActual_Raw;

                  PT_CAN_msg0x7E9_DQ500_OilPressureActual_Raw = (PT_CAN_msg0x7E9_DQ500_OilPressureActual_LSB + (PT_CAN_msg0x7E9_DQ500_OilPressureActual_MSB * 256));
                  PT_CAN_msg0x7E9_DQ500_OilPressureActual_Int = PT_CAN_msg0x7E9_DQ500_OilPressureActual_Raw;
                  
                  PT_CAN_TCU_0x7E9_PressureActuals_CF_Num = 1;
                  // Serial.print("CFNum = 1");

                }
                if( PT_CAN_TCU_0x7E9_PressureActuals_CF_Num == PT_CAN_TCU_0x7E9_PressureActuals_CF_Max){
                  if( PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22 == 2){
                      PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22 = 0;
                    // Serial.print("CFMax: ");

                      PT_CAN_TCU_0x7E9_PressureActuals_CF_Num = 0;
                      // PT_CAN_f_sid22_TCU_PressureActuals_multi = 0;   
                              //  Serial.print("CFMax: ");
                              //  Serial.print(CFNum);

                      // TimeSinceSTATFC1_PT_CAN_OLD_TCU_ReqSet1 = 0;
                      PT_CAN_DQ500_DAQ_OLD_Set1_SequenceComplete = true;
                    // Serial.print("TimeSinceSTATFCreset: "); Serial.print(TimeSinceSTATFC1_PT_CAN_OLD_TCU_ReqSet1); Serial.println(TimeSinceSTATFC1_PT_CAN_OLD_TCU_ReqSet1);
                    // Serial.print("PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22: "); Serial.print(PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22); Serial.println("");
                  }
            
              // Serial.println();

            }
            }


                if (TimeSinceSTATFC1_PT_CAN_OLD_TCU_ReqSet1 > 5000 && DS1_ECU_LoggingConfigActive > 8000 && PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22 > 0) {

                      Serial.print("Sending PT_CAN_TCUSet1Msg1 due to Timeout  ");  Serial.println(" "); 

                PT_CAN_DQ500_DAQ_OLD_Set1_SequenceComplete = true;
                PT_CAN_TCU_0x7E9_PressureActuals_STAT_FC_22 = 0;
                PT_CAN_TCU_0x7E9_PressureActuals_CF_Num = 0;
                PT_CAN_f_sid22_TCU_PressureActuals_multi = 0;   
                // canTx_PT_CAN_22_TCU_OLD_Set1_PressureActualsOnly(); // Query data from CAN BUS via OBD
                TimeSinceSTATFC1_PT_CAN_OLD_TCU_ReqSet1 = 0;
                PT_CAN_msg0x7E9_buf0_Raw = 0;
                PT_CAN_msg0x7E9_buf1_Raw = 0;
                PT_CAN_msg0x7E9_buf2_Raw = 0;
                PT_CAN_msg0x7E9_buf3_Raw = 0;
                PT_CAN_msg0x7E9_buf4_Raw = 0;
                PT_CAN_msg0x7E9_buf5_Raw = 0;
                PT_CAN_msg0x7E9_buf6_Raw = 0;
                PT_CAN_msg0x7E9_buf7_Raw = 0;



                canTx_PT_CAN_22_TCU_OLD_Set1_PressureActualsOnly();
                              PT_CAN_DQ500_DAQ_OLD_Set1_SequenceComplete = false;


                }


              PT_CAN_TCU_OLD_Clutch1_PressureActual_Int = PT_CAN_msg0x7E9_Clutch1_PressureActual_Raw + 4;
              PT_CAN_TCU_OLD_Clutch2_PressureActual_Int = PT_CAN_msg0x7E9_Clutch2_PressureActual_Raw + 4;
              PT_CAN_TCU_DQ500_OilPressureActual_Int = PT_CAN_msg0x7E9_DQ500_OilPressureActual_Raw + 4;


  #pragma endregion

#pragma endregion





#pragma region // CAN Bitshift Coordinations - TCU - Pressure Actuals

    PT_CAN_TCU_OLD_Clutch1_PressureActual_Int = PT_CAN_msg0x7E9_Clutch1_PressureActual_Raw + 4;
    PT_CAN_TCU_OLD_Clutch2_PressureActual_Int = PT_CAN_msg0x7E9_Clutch2_PressureActual_Raw + 4;
    PT_CAN_TCU_DQ500_OilPressureActual_Int = PT_CAN_msg0x7E9_DQ500_OilPressureActual_Raw + 4;


    lsb_PT_CAN_TCU_Clutch1_PressureActual_Int = PT_CAN_TCU_OLD_Clutch1_PressureActual_Int & 0x00FF;
    msb_PT_CAN_TCU_Clutch1_PressureActual_Int = (PT_CAN_TCU_OLD_Clutch1_PressureActual_Int >> 8) & 0x00FF;

    lsb_PT_CAN_TCU_Clutch2_PressureActual_Int = PT_CAN_TCU_OLD_Clutch2_PressureActual_Int & 0x00FF;
    msb_PT_CAN_TCU_Clutch2_PressureActual_Int = (PT_CAN_TCU_OLD_Clutch2_PressureActual_Int >> 8) & 0x00FF;


    // lsb_PT_CAN_TCU_Clutch1_PressureActual_Int = PT_CAN_TCU_Clutch1_PressureActual_Int & 0x00FF;
    // msb_PT_CAN_TCU_Clutch1_PressureActual_Int = (PT_CAN_TCU_Clutch1_PressureActual_Int >> 8) & 0x00FF;

    // lsb_PT_CAN_TCU_Clutch2_PressureActual_Int = PT_CAN_TCU_Clutch2_PressureActual_Int & 0x00FF;
    // msb_PT_CAN_TCU_Clutch2_PressureActual_Int = (PT_CAN_TCU_Clutch2_PressureActual_Int >> 8) & 0x00FF;

    lsb_PT_CAN_TCU_DQ500_OilPressureActual_Int = PT_CAN_TCU_OLD_DQ500_OilPressureActual_Int & 0x00FF;
    msb_PT_CAN_TCU_DQ500_OilPressureActual_Int = (PT_CAN_TCU_OLD_DQ500_OilPressureActual_Int >> 8) & 0x00FF;


  

#pragma endregion




}



