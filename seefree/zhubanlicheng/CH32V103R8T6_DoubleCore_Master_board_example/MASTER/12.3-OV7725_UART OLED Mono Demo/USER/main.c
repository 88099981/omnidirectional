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


#include "headfile.h"

int main(void)
{
    DisableGlobalIRQ();
    board_init();   //务必保留，本函数用于初始化MPU 时钟 调试串口


    //OLED屏幕方向为横屏模式,屏幕方向可以在SEEKFREE_OLED.H的OLED_DISPLAY_DIR宏定义修改
    oled_init();    //oled初始化
    oled_p6x8str(0,0,"SEEKFREE OV7725");
    oled_p6x8str(0,1,"Initializing...");
    //如果屏幕没有任何显示，请检查屏幕接线

    ov7725_uart_init();  //小钻风初始化
    //如果采集到的图像不正确:
    //1.请查看OV7725_IDCode是否等于0x21，若不等于，则检查SCL和SDA是否接好
    //2.如果使用主板，请检查是否电池连接OK?
    //3.如果图像只采集一次，请检查场信号(VSY)是否连接OK?

    oled_p6x8str(0,1,"     OK...     ");
    systick_delay_ms(500);
    oled_p6x8str(0,0,"               ");    //清屏
    oled_p6x8str(0,1,"               ");    //清屏

    EnableGlobalIRQ(0);
    while(1)
    {
        if(ov7725_uart_finish_flag)
        {
            ov7725_uart_finish_flag = 0;
            Image_Decompression(ov7725_uart_image_bin[0], ov7725_uart_image_dec[0]);     //解压图像
            oled_dis_bmp(OV7725_UART_H, OV7725_UART_W, ov7725_uart_image_dec[0], 0x7F);  //显示图像
        }
    }
}


