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
int16 encoder_data[2];
int main(void)
{
    DisableGlobalIRQ();
    board_init();   //务必保留，本函数用于初始化MPU 时钟 调试串口

    //这里的编码器仅支持正交解码
    //CH32V103R8T6  单核主板推荐角度编码器，双核主板推荐正交解码编码器，不推荐带方向的编码器
    printf("encoder example");

    timer_quad_init(TIMER_2, TIMER2_CHA_A15, TIMER2_CHB_B3);
    timer_quad_init(TIMER_3, TIMER3_CHA_B4, TIMER3_CHB_B5);


    EnableGlobalIRQ(0);
    for(;;)
    {

        encoder_data[0] = timer_quad_get(TIMER_2);      //编码器取值
        encoder_data[1] = timer_quad_get(TIMER_3);      //编码器取值
        timer_quad_clear(TIMER_2);                      //清空计数器
        timer_quad_clear(TIMER_3);                      //清空计数器


        printf("encoder_data[0] = %d\r\n", encoder_data[0]);
        printf("encoder_data[1] = %d\r\n", encoder_data[1]);


//        data_conversion(encoder_data[0], encoder_data[1], encoder_data[2], encoder_data[3], virtual_scope_data); //利用虚拟示波器发送数据
//        uart_putbuff(UART_1, virtual_scope_data, 10);  //数据转换完成后，使用串口发送将数组的内容发送出去

        systick_delay_ms(10);                                                   //延时10ms
    }


}
