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

int16 enc;
uint8 dir_mode = 0x00;


int main(void)
{
    DisableGlobalIRQ();
    board_init();   //务必保留，本函数用于初始化MPU 时钟 调试串口

    gpio_init(C0, GPO, 1, GPIO_PIN_CONFIG);
    gpio_init(C1, GPO, 1, GPIO_PIN_CONFIG);


    //CH32V103R8T6我们推荐使用正交解码编码器。
    //CH32V103R8T6我们推荐使用正交解码编码器。
    //CH32V103R8T6我们推荐使用正交解码编码器。

    timer_quad_init(TIMER_2, TIMER2_CHA_A0, TIMER2_CHB_A1);

    EnableGlobalIRQ(0);


    while(1)
    {
        enc = timer_quad_get(TIMER_2);      //编码器取值
        timer_quad_clear(TIMER_2);          //清空定时器的计数器值
        printf("enc = %d\r\n", enc);
        systick_delay_ms(500);
    }


}



