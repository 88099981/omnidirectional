#ifndef _hcar_adc_h
#define _hcar_adc_h
#include "common.h"
#include "zf_adc.h"

#define ADC_CH1 ADC_IN4_A4
#define ADC_CH2 ADC_IN6_A6
#define ADC_CH3 ADC_IN8_B0
#define ADC_CH4 ADC_IN9_B1

//±äÁ¿
extern uint16  ad_value1;
extern uint16  ad_value2;
extern uint16  ad_value3;
extern uint16  ad_value4;
extern uint16  guiyi_ad1,guiyi_ad2,guiyi_ad3,guiyi_ad4;
extern int16 ad_error;

//º¯Êý
void hcar_adc_init(void);
void hcar_adc_get(void);
void hcar_stop(void);

#endif
