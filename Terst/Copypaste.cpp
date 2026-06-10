        if (fromECU_frame.id == 0x784){ // Sniff the desired EngTQ Limit or EngTQ Multiplier, which would originate from the TFT Master Module 
                // Serial.println("tftcan3 recieved");


                  PT_CAN_msg0x784_buf0_Raw = fromECU_frame.buf[0];
                  PT_CAN_msg0x784_buf1_Raw = fromECU_frame.buf[1];
                  PT_CAN_msg0x784_buf2_Raw = fromECU_frame.buf[2];
                  PT_CAN_msg0x784_buf3_Raw = fromECU_frame.buf[3];
                  PT_CAN_msg0x784_buf4_Raw = fromECU_frame.buf[4];
                  PT_CAN_msg0x784_buf5_Raw = fromECU_frame.buf[5];
                  PT_CAN_msg0x784_buf6_Raw = fromECU_frame.buf[6];
                  PT_CAN_msg0x784_buf7_Raw = fromECU_frame.buf[7];

                  PT_CAN_msg0x784_FromECU_buf0 = PT_CAN_msg0x784_buf0_Raw;
                  PT_CAN_msg0x784_FromECU_buf1 = PT_CAN_msg0x784_buf1_Raw;
                  PT_CAN_msg0x784_FromECU_buf2 = PT_CAN_msg0x784_buf2_Raw;
                  PT_CAN_msg0x784_FromECU_buf3 = PT_CAN_msg0x784_buf3_Raw;
                  PT_CAN_msg0x784_FromECU_buf4 = PT_CAN_msg0x784_buf4_Raw;
                  PT_CAN_msg0x784_FromECU_buf5 = PT_CAN_msg0x784_buf5_Raw;
                  PT_CAN_msg0x784_FromECU_buf6 = PT_CAN_msg0x784_buf6_Raw;
                  PT_CAN_msg0x784_FromECU_buf7 = PT_CAN_msg0x784_buf7_Raw;


                  if(PT_CAN_msg0x784_buf0_Raw == 0x1) {

                      MITM_TCU_EngTQmod_TQ_Multiplier_from_msg0x784_Int_8bit = (PT_CAN_msg0x784_FromECU_buf1);
                      // MITM_TCU_EngTQmod_TQ_Multiplier_from_msg0x784_Float = MITM_TCU_EngTQmod_TQ_Multiplier_from_msg0x784_Int_8bit / 100.0;

                      MITM_TCU_EngTQmod_TQ_Multiplier_RampOutSpeed = (PT_CAN_msg0x784_FromECU_buf2);


                    }

                    // MITM_TCU_EngTQmod_TQ_Multiplier_from_msg0x784_Int_8bit = 50; // for testing
                    // MITM_TCU_EngTQmod_TQ_Multiplier_from_msg0x784_Float = MITM_TCU_EngTQmod_TQ_Multiplier_from_msg0x784_Int_8bit / 100.0; // temporary testing

                    // MITM_TCU_EngTQmod_TQ_Limit_from_msg0x784_Int_8bit = 22; // for testing - this would be multiplied by 10 to arrive at 220Nm torque limit.

              }

