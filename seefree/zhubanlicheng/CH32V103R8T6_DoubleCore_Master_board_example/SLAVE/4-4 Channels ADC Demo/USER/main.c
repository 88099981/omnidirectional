/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file            main
 * @company         成都逐飞科技有限公司
 * @author          逐飞科技(QQ790875685)
 * @version         查看doc内version文件 版本说明
 * @Software        MounRiver Studio V1.3
 * @Target core     CH32V103R8T6
 * @Taobao          https://seekfree.taobao.com/
 * @date            2020-12-04
 ********************************************************************************************************************/
//整套推荐IO查看Projecct文件夹下的TXT文本

//打开新的工程或者工程移动了位置务必执行以下操作
//右键单击工程，选择刷新


// 本例程推荐使用1.8寸TFT显示屏观察效果，若您没有1.8寸TFT屏，可以通过调试时的Watch窗口观察按键状态。
// 通过在线调试功能，查看 key1_status key2_status key3_status  的数值即可。
// 在本例程中，当按键按下时，按键状态值就会加一。

#include "headfile.h"

#define ADC_CH1 ADC_IN4_A4
#define ADC_CH2 ADC_IN6_A6
#define ADC_CH3 ADC_IN8_B0
#define ADC_CH4 ADC_IN9_B1


uint16  ad_value1;
uint16  ad_value2;
uint16  ad_value3;
uint16  ad_value4;

int main(void)
{
    DisableGlobalIRQ();
    board_init();   //务必保留，本函数用于初始化MPU 时钟 调试串口

    adc_init(ADC_CH1);
    adc_init(ADC_CH2);
    adc_init(ADC_CH3);
    adc_init(ADC_CH4);

    EnableGlobalIRQ(0);
    for(;;)
    {
        //采集ADC数据
        ad_value1 = adc_mean_filter(ADC_CH1, ADC_8BIT, 10);
        ad_value2 = adc_mean_filter(ADC_CH2, ADC_8BIT, 10);
        ad_value3 = adc_mean_filter(ADC_CH3, ADC_8BIT, 10);
        ad_value4 = adc_mean_filter(ADC_CH4, ADC_8BIT, 10);

        //通过在线调试功能，查看ad_value1 ad_value2 ad_value3 ad_value4的数值即可。
        //使用方法，右键点击变量例如ad_value1选择添加查看表达式即可在watch窗口看到变量实时值。

        //有人总是问，为啥ADC引脚悬空有数值呢？不应该是0？
        //我的回答：是的，悬空的时候是有值，这里我只给出答案，具体为什么大家自己研究ADC的结构即可明白。

        systick_delay_ms(10);


    }


}


