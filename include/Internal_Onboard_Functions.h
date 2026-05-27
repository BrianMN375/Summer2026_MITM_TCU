

#ifndef Internal_Onboard_Functions_H 
#define Internal_Onboard_Functions_H


#ifdef BoardFamily_BasicDAZA

void Internal_Onboard_Functions_BasicDAZA_init();

#endif

#ifdef BoardFamily_KitchenSink

void Internal_Onboard_Functions_KitchenSink_init();

#endif

void MCU_init_Send_FW_Version_Serial();

void do_MCU_CoreTempMonitoring();



#endif 
