/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00               东秦全向三队 编写                                                               00**
**00             队员 ：陈诏栋 刘坤龙 石柳苗                                                   00**
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00            函数作用：  go                             00**
**0000000000000000000000000000000000000000000000000000000**
**********************************************************/
#include "hcar_adc.h"
#include "middline.h"

uint16  ad_value1;
uint16  ad_value2;
uint16  ad_value3;
uint16  ad_value4;

void hcar_adc_init(void)
{
    adc_init(ADC_CH1);
    adc_init(ADC_CH2);
    adc_init(ADC_CH3);
    adc_init(ADC_CH4);
}

void hcar_adc_get(void)
{
    ad_value1 = adc_mean_filter(ADC_CH1, ADC_8BIT, 5);
    ad_value2 = adc_mean_filter(ADC_CH2, ADC_8BIT, 5);
    ad_value3 = adc_mean_filter(ADC_CH3, ADC_8BIT, 5);
    ad_value4 = adc_mean_filter(ADC_CH4, ADC_8BIT, 5);
}

void hcar_stop(void)
{
    if(ad_value1 + ad_value2 + ad_value3 + ad_value4 <= 30)
    {
        break_flag=1;
    }
}
