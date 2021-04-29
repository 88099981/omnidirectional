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
#include "hcar_element.h"
#include "hcar_init.h"
#include "hcar_adc.h"
#include "SEEKFREE_ICM20602.h"
#include "middline.h"
#include "SEEKFREE_18TFT.h"

float last_z=0.0;
void hcar_start(void)
{
    aim_x=120;
    hcar_adc_get();
    while((ad_value3 + ad_value4  >= ad_value1 + ad_value2+40 )||(ad_value1 + ad_value2 + ad_value3 + ad_value4 <= 200))
    {
        ad_value1 = adc_mean_filter(ADC_CH1, ADC_8BIT, 5);
        ad_value2 = adc_mean_filter(ADC_CH2, ADC_8BIT, 5);
        ad_value3 = adc_mean_filter(ADC_CH3, ADC_8BIT, 5);
        ad_value4 = adc_mean_filter(ADC_CH4, ADC_8BIT, 5);
        //hcar_adc_get();
        /*aim_x-=10;*/

    }
    aim_x=0;
    aim_y=180;
    //turn_sign=1;
}
void hcar_threefork30()
{
    int16 time=230;
    sancha_first++;
    if(sancha_first==1)
        {
            aim_y=0;
            turn_sum=0;
            aim_z=200;
            get_icm20602_gyro_spi();
            while(turn_sum>-3200)
            {
                get_icm20602_gyro_spi();
                turn_sum+=icm_gyro_z/10;
                lcd_showstr(0,0,"aim_x:");
                lcd_showint16(12*8,0,aim_x);
                lcd_showstr(0,1,"aim_y:");
                lcd_showint16(12*8,1,aim_y);
                lcd_showstr(0,2,"aim_z:");
                lcd_showint16(12*8,2,aim_z);
                lcd_showstr(0,3,"icm_acc_x:");
                lcd_showint16(12*8,3,icm_acc_x);
                lcd_showstr(0,4,"icm_acc_y:");
                lcd_showint16(12*8,4,icm_acc_y);
                lcd_showstr(0,5,"icm_acc_z:");
                lcd_showint16(12*8,5,icm_acc_z);
                lcd_showstr(0,6,"turn_sum:");
                lcd_showint32(12*8,6,turn_sum,5);
            }

            while(time>=0)
            {
                aim_x=-100;
                aim_z = -fuhe_speed;

                lcd_showstr(0,0,"aim_x:");
                lcd_showint16(12*8,0,aim_x);
                lcd_showstr(0,1,"aim_y:");
                lcd_showint16(12*8,1,aim_y);
                lcd_showstr(0,2,"aim_z:");
                lcd_showint16(12*8,2,aim_z);
                lcd_showstr(0,3,"icm_acc_x:");
                lcd_showint16(12*8,3,icm_acc_x);
                lcd_showstr(0,4,"icm_acc_y:");
                lcd_showint16(12*8,4,icm_acc_y);
                lcd_showstr(0,5,"icm_acc_z:");
                lcd_showint16(12*8,5,icm_acc_z);
                lcd_showstr(0,6,"turn_sum:");
                lcd_showint32(12*8,6,turn_sum,5);
                time--;
            }
            aim_x=0;
            //aim_y=200;
            turn_sum=0;
            while(turn_sum<1000)
            {

                aim_z=-200;
                get_icm20602_gyro_spi();
                turn_sum+=icm_gyro_z/10;
                lcd_showstr(0,0,"aim_x:");
                lcd_showint16(12*8,0,aim_x);
                lcd_showstr(0,1,"aim_y:");
                lcd_showint16(12*8,1,aim_y);
                lcd_showstr(0,2,"aim_z:");
                lcd_showint16(12*8,2,aim_z);
                lcd_showstr(0,3,"icm_acc_x:");
                lcd_showint16(12*8,3,icm_acc_x);
                lcd_showstr(0,4,"icm_acc_y:");
                lcd_showint16(12*8,4,icm_acc_y);
                lcd_showstr(0,5,"icm_acc_z:");
                lcd_showint16(12*8,5,icm_acc_z);
                lcd_showstr(0,6,"turn_sum:");
                lcd_showint32(12*8,6,turn_sum,5);
            }
            //bb_time=50;
            //sancha_first=1;
        }
    else if(sancha_first==2)
    {
        aim_y=0;
        turn_sum=0;


        while(time>=0)
        {
            aim_x=-100;
            aim_z = -fuhe_speed;

            lcd_showstr(0,0,"aim_x:");
            lcd_showint16(12*8,0,aim_x);
            lcd_showstr(0,1,"aim_y:");
            lcd_showint16(12*8,1,aim_y);
            lcd_showstr(0,2,"aim_z:");
            lcd_showint16(12*8,2,aim_z);
            lcd_showstr(0,3,"icm_acc_x:");
            lcd_showint16(12*8,3,icm_acc_x);
            lcd_showstr(0,4,"icm_acc_y:");
            lcd_showint16(12*8,4,icm_acc_y);
            lcd_showstr(0,5,"icm_acc_z:");
            lcd_showint16(12*8,5,icm_acc_z);
            lcd_showstr(0,6,"turn_sum:");
            lcd_showint32(12*8,6,turn_sum,5);
            time--;
        }
        aim_x=0;
        //aim_y=200;
        turn_sum=0;
        while(turn_sum<1000)
        {

            aim_z=-200;
            get_icm20602_gyro_spi();
            turn_sum+=icm_gyro_z/10;
            lcd_showstr(0,0,"aim_x:");
            lcd_showint16(12*8,0,aim_x);
            lcd_showstr(0,1,"aim_y:");
            lcd_showint16(12*8,1,aim_y);
            lcd_showstr(0,2,"aim_z:");
            lcd_showint16(12*8,2,aim_z);
            lcd_showstr(0,3,"icm_acc_x:");
            lcd_showint16(12*8,3,icm_acc_x);
            lcd_showstr(0,4,"icm_acc_y:");
            lcd_showint16(12*8,4,icm_acc_y);
            lcd_showstr(0,5,"icm_acc_z:");
            lcd_showint16(12*8,5,icm_acc_z);
            lcd_showstr(0,6,"turn_sum:");
            lcd_showint32(12*8,6,turn_sum,5);
        }
        //bb_time=50;
        //sancha_first=0;
    }




}
