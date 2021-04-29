#ifndef __LQ_ENC_H_
#define __LQ_ENC_H_
#include "include.h"

extern volatile int32_t RAllPulse;
extern int16_t ECPULSE1;
extern int16_t ECPULSE2;
extern int16_t ECPULSE3;
extern int16_t ECPULSE4;
void EncInit (void);
int Read_Encoder(u8 TIMX);
void TestEncoderOLED(void);
void TestEncoderTFT(void);
#endif

//



