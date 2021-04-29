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
    board_init();       //务必保留，本函数用于初始化MPU 时钟 调试串口


    //显示模式设置为3  横屏模式
    //显示模式在SEEKFREE_IPS114_SPI.h文件内的IPS114_DISPLAY_DIR宏定义设置
    ips114_init();     //初始化TFT屏幕
    ips114_showstr(0, 0, "SEEKFREE MT9V03x");
    ips114_showstr(0, 1, "Initializing...");
    //如果屏幕没有任何显示，请检查屏幕接线

    mt9v03x_init();              //摄像头初始化
    //如果屏幕一直显示初始化信息，请检查摄像头接线
    //如果使用主板，一直卡在while(!uart_receive_flag)，请检查是否电池连接OK?
    //如果图像只采集一次，请检查场信号(VSY)是否连接OK?
    ips114_showstr(0, 1, "OK...");
    systick_delay_ms(500);
    //总中断最后开启

    EnableGlobalIRQ(0);
    while(1)
    {
        if(mt9v03x_finish_flag)
        {
            mt9v03x_finish_flag = 0;
            //使用缩放显示函数，根据原始图像大小 以及设置需要显示的大小自动进行缩放或者放大显示
            ips114_displayimage032_zoom(mt9v03x_image[0], MT9V03X_W, MT9V03X_H, IPS114_X_MAX, IPS114_Y_MAX);
        }
    }
}


