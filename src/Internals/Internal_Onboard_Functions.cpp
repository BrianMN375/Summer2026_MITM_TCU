
#include <global_vars.h>
#include <Arduino.h> /* we need arduino functions to implement this */
#include <Internal_Onboard_Functions.h>
#include <InternalTemperature.h>


float InternalTemp;
int InternalCPUTemp;
unsigned int CPU_SpeedMhz;
elapsedMillis loopDelay_InternalMCUTemp;
float CPU_Temp;
unsigned char lsbInternalCPUTemp;


void do_MCU_CoreTempMonitoring() {


InternalTemp = InternalTemperature.readTemperatureC();
CPU_Temp = tempmonGetTemp();
InternalCPUTemp = InternalTemp;
CPU_SpeedMhz = ((F_CPU_ACTUAL)/1000000);
// Serial.print("CPU_Temp: ");  Serial.print("\t");   Serial.print(CPU_Temp); Serial.print("\t");

  lsbInternalCPUTemp = (InternalCPUTemp & 0x00FF);




}