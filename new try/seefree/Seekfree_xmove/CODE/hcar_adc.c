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


vuint8 x_move_time=0;
vuint8 ruhuan=0,chuhuan=0;
vuint8 chuhuan_delay=0;
vuint8 ruhuan_delay=0;
vuint8 ruhuan_wait=0;
uint16  ad_value1;
uint16  ad_value2;
uint16  ad_value3;
uint16  ad_value4;
uint16  ad_max[8];
uint16  ad_min[8];
int16 ad_speed=0;
float lasttime_ad=0.0;
//uint16  guiyi_ad1,guiyi_ad2,guiyi_ad3,guiyi_ad4;
float ad_error=0.0;
float kp_ad=0.0, kd_ad=0.0;
float ad_left=0.0, ad_right=0.0;

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

    ad_value1 = adc_mean_filter(ADC_CH1, ADC_8BIT, 5);
    ad_value2 = adc_mean_filter(ADC_CH2, ADC_8BIT, 5);
    ad_value3 = adc_mean_filter(ADC_CH3, ADC_8BIT, 5);
    ad_value4 = adc_mean_filter(ADC_CH4, ADC_8BIT, 5);

    if(chuhuan_delay>0)
    {
        chuhuan_delay--;
    }
    if(ruhuan_delay>0)
    {
        ruhuan_delay--;
    }
    if(ruhuan_wait>0)
    {
        ruhuan_wait--;
        if(ruhuan_wait<=0)
        {
            zuo_yuanhuan_flag=0;
            you_yuanhuan_flag=0;
        }
    }
    if(cross_flag==1)
    {
//        ad_left=ad_value2;
//        ad_right=ad_value3;
        bb_time=10;
    }
    if(ad_value4+ad_value3+ad_value2+ad_value1>330&&chuhuan_delay<=0&&ruhuan_delay<=0)
    {
        if(zuo_yuanhuan_flag==0&&you_yuanhuan_flag==0)
        {
            ruhuan++;
            if(ad_value1-ad_value4>30&&ad_value1>90)//zuo
            {
                if(ruhuan>=7)
                {
                    zuo_yuanhuan_flag=1;
                    ruhuan=0;
                    bb_time=20;
                    ruhuan_wait=120;
                    ruhuan_delay=80;//防止二次误判
                }
            }
            else if(ad_value4-ad_value1>20&&ad_value4>90)//you
            {
                if(ruhuan>=7)
                {
                    you_yuanhuan_flag=1;
                    ruhuan=0;
                    bb_time=20;
                    ruhuan_wait=120;
                    ruhuan_delay=80;
                }
            }
            else
            {
                ad_left=ad_value1*0.25+ad_value2*0.75;
                ad_right=ad_value3*0.75+ad_value4*0.25;
            }
        }
        else
        {
            chuhuan++;
            if(zuo_yuanhuan_flag==1&&chuhuan>=10)
            {
                zuo_yuanhuan_flag=0;
                //camera_down=100;
                ad_left=ad_value1*0.5+ad_value2*0.25;
                ad_right=ad_value3*0.7+ad_value4*0.7;
                chuhuan=0;
                bb_time=60;
                chuhuan_delay=100;
            }
            else if(you_yuanhuan_flag==1&&chuhuan>=10)
            {
                you_yuanhuan_flag=0;
                //camera_down=100;
                ad_left=ad_value1*0.7+ad_value2*0.7;
                ad_right=ad_value3*0.5+ad_value4*0.25;
                chuhuan=0;
                bb_time=60;
                chuhuan_delay=100;
            }

        }
    }
    else if(zuo_yuanhuan_flag==1)
    {
        ad_left=ad_value1*0.5+ad_value2*0.5;
        ad_right=ad_value3*0.5;
    }
    else if(you_yuanhuan_flag==1)
    {
        ad_left=ad_value2*0.5;
        ad_right=ad_value3*0.5+ad_value4*0.5;
    }
    else
    {
        ruhuan=0;
        chuhuan=0;
        ad_left=ad_value1*0.25+ad_value2*0.75;
        ad_right=ad_value3*0.75+ad_value4*0.25;
    }

//    if(x_move_time>0)
//    {
//        x_move_time--;
//        if(camera_down==1&&x_move_time==0)
//        {
//            camera_down==0;
//        }
//        //aim_x=100;
//
//    }

//    sqrt_left=sqrt(ad_left);
//    sqrt_right=sqrt(ad_right);

    ad_error=(ad_left-ad_right)*100/(ad_left+ad_right);
    ad_speed= kp_ad*ad_error + kd_ad*(ad_error-lasttime_ad);
    lasttime_ad=ad_error;
}
//uint8 stop_cnt=0;
void hcar_stop(void)
{
//    if(ad_value1 + ad_value2 + ad_value3 + ad_value4 <= 40)
//    {
//        stop_cnt++;
//        if(stop_cnt>=250)
//        {
//            break_flag=1;
//            stop_cnt=250;
//        }
//    }
//    else
//    {
//        stop_cnt--;
//        if(stop_cnt<=0)
//        {
//            stop_cnt=0;
//            break_flag=0;
//        }
//    }
}
