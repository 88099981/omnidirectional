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

    //IPS屏幕方向为横屏模式,屏幕方向可以在SEEKFREE_SPI114_SPI.H的IPS114_DISPLAY_DIR宏定义修改
    ips114_init();                 //1.14寸IPS屏幕初始化
    ips114_showstr(0,0,(uint8 *)"SEEKFREE OV7725");
    ips114_showstr(0,1,(uint8 *)"Initializing...");
    //如果屏幕没有任何显示，请检查屏幕接线

    ov7725_uart_init();              //摄像头初始化
    //如果采集到的图像不正确:
    //1.请查看摄像头的配置串口与摄像头连接是否正确
    //2.如果使用主板，请检查是否电池连接OK?
    //3.如果图像只采集一次，请检查场信号(VSY)是否连接OK?

    ips114_showstr(0,1,(uint8 *)"     OK...     ");
    systick_delay_ms(500);

    //总中断最后开启
    EnableGlobalIRQ(0);;
    while (1)
    {
        //此处编写需要循环执行的代码
        if(ov7725_uart_finish_flag)
        {
           ips114_displayimage7725(ov7725_uart_image_bin[0], OV7725_UART_W, OV7725_UART_H);
           ov7725_uart_finish_flag = 0;
        }
    }
}


