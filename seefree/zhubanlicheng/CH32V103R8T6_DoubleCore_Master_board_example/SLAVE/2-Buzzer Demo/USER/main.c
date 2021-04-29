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

#define BEEP_PIN   D2       //定义蜂鸣器引脚


int main(void)
{

    DisableGlobalIRQ();
    board_init();   //务必保留，本函数用于初始化MPU 时钟 调试串口

    systick_delay_ms(300);  //延时300ms，等待主板其他外设上电成功


    //初始化蜂鸣器引脚
    gpio_init(BEEP_PIN,GPO,0,GPIO_PIN_CONFIG);

    EnableGlobalIRQ(0);

    while(1)
    {
        gpio_set(BEEP_PIN,0);//关闭蜂鸣器
        systick_delay_ms(1000);
        gpio_set(BEEP_PIN,1);//打开蜂鸣器
        systick_delay_ms(1000);
    }


}
