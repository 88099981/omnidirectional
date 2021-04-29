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
#include "hcar_init.h"

#define kp_ad 4.0
#define kd_ad 4.0

uint16  ad_value1;
uint16  ad_value2;
uint16  ad_value3;
uint16  ad_value4;
uint16  ad_max[8];
uint16  ad_min[8];
float lasttime_ad=0.0;
//uint16  guiyi_ad1,guiyi_ad2,guiyi_ad3,guiyi_ad4;
int16 ad_error=0;

void hcar_adc_init(void)
{
    adc_init(ADC_CH1);
    adc_init(ADC_CH2);
    adc_init(ADC_CH3);
    adc_init(ADC_CH4);
}

void hcar_adc_get(void)
{
    //float sqrt_left=0.0,sqrt_right=0.0;
    float ad_left, ad_right;
    ad_value1 = adc_mean_filter(ADC_CH1, ADC_8BIT, 5);
    ad_value2 = adc_mean_filter(ADC_CH2, ADC_8BIT, 5);
    ad_value3 = adc_mean_filter(ADC_CH3, ADC_8BIT, 5);
    ad_value4 = adc_mean_filter(ADC_CH4, ADC_8BIT, 5);

//    if (ad_value1 < ad_min[1])        ad_min[1] = ad_value1;
//    else if (ad_value1 > ad_max[1])   ad_max[1] = ad_value1;
//
//    if (ad_value2 < ad_min[2])        ad_min[2] = ad_value2;
//    else if (ad_value2 > ad_max[2])   ad_max[2] = ad_value2;
//
//
//    if (ad_value3 < ad_min[3])        ad_min[3] = ad_value3;
//    else if (ad_value3 > ad_max[3])   ad_max[3] = ad_value3;
//
//    if (ad_value4 < ad_min[4])        ad_min[4] = ad_value4;
//    else if (ad_value4 > ad_max[4])   ad_max[4] = ad_value4;
//
//    guiyi_ad1  = (ad_value1 - ad_min[1]) * 100 / (ad_max[1] - ad_min[1]);
//    guiyi_ad2  = (ad_value2 - ad_min[2]) * 100 / (ad_max[2] - ad_min[2]);
//    guiyi_ad3  = (ad_value3 - ad_min[3]) * 100 / (ad_max[3] - ad_min[3]);
//    guiyi_ad4  = (ad_value4 - ad_min[4]) * 100 / (ad_max[4] - ad_min[4]);
    ad_left=ad_value1*0.25+ad_value2*0.75;
    ad_right=ad_value3*0.75+ad_value4*0.25;
//    sqrt_left=sqrt(ad_left);
//    sqrt_right=sqrt(ad_right);

    ad_error=(ad_left-ad_right)*100/(ad_left+ad_right);
    aim_z += kp_ad*ad_error + kd_ad*(ad_error-lasttime_ad);
    lasttime_ad=ad_error;
}

void hcar_stop(void)
{
    if(ad_value1 + ad_value2 + ad_value3 + ad_value4 <= 30)
    {
        break_flag=1;
    }
}
