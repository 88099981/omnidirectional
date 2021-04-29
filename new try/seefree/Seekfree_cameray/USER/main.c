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

#ifdef DOUBLE_CORE_HOST
int main(void)
{
    DisableGlobalIRQ();
    board_init();       //务必保留，本函数用于初始化MPU 时钟 调试串口

    systick_delay_ms(300);  //延时300ms，等待主板其他外设上电成功
    hcar_init();

    gpio_interrupt_init(A0, RISING, GPIO_INT_CONFIG);       //A0初始化为GPIO 上升沿触发
    nvic_init(EXTI0_IRQn, 0, 1, ENABLE);                    //EXTI0优先级配置，抢占优先级1，次优先级1

    //显示模式设置为3  竖屏模式
    //显示模式在SEEKFREE_18TFT.h文件内的TFT_DISPLAY_DIR宏定义设置
    lcd_init();         //初始化TFT屏幕
    lcd_showstr(0,0,"SEEKFREE MT9V03x");
    lcd_showstr(0,1,"Initializing...");
    //如果屏幕没有任何显示，请检查屏幕接线

    //mt9v03x_init(); //初始化摄像头 使用CSI接口
    //如果屏幕一直显示初始化信息，请检查摄像头接线
    //如果使用主板，一直卡在while(!uart_receive_flag)，请检查是否电池连接OK?
    //如果图像只采集一次，请检查场信号(VSY)是否连接OK?
    mt9v03x_init();


    lcd_showstr(0,1,"     OK...     ");
    systick_delay_ms(500);
    get_cut_array();
    EnableGlobalIRQ(0);
    while(1)
    {

        if(mt9v03x_finish_flag)
        {
            mt9v03x_finish_flag = 0;
            cut_image_to_img();
            binary_img();
            //zebra_crossing();
            get_new_edge();
            set_middline_black();
            road_seek();
            //使用缩放显示函数，根据原始图像大小 以及设置需要显示的大小自动进行缩放或者放大显示
            lcd_displayimage032_zoom(img[0], 80, 60, 160, 128);

        }

    }
}

#elif  defined DOUBLE_CORE_SLAVE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////双机通信之从机，发送编码器和或摄像头算法结果数据，插在左边卡槽
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(void)//双机通信之从机，发送编码器和或摄像头算法结果数据，插在左边卡槽
{


    // 系统及延时函数初始化
    DisableGlobalIRQ();
    board_init();           //务必保留，本函数用于初始化MPU 时钟 调试串口
    hcar_init();
    lcd_init();
    gpio_init(A0, GPO, 0, GPIO_PIN_CONFIG);                 //同步引脚初始化
    timer_pit_interrupt_ms(TIMER_4, 20);                    //定时器4初始化

//    seekfree_wireless_init();
//    uint8 test[]="This is wireless application.";
//    seekfree_wireless_send_buff(test, sizeof(test)-1);
    //int16 time=0;
    uart_rx_irq(UART_3, ENABLE);                            //默认抢占优先级0 次优先级0。
    nvic_init((IRQn_Type)(53 + UART_3), 0, 0, ENABLE);      //将串口3的抢占优先级设置为最高，次优先级设置为最高。

    EnableGlobalIRQ(0);
    while(1)
    {
//        time++;
        //将接收到的从机数据显示到屏幕上。
//        lcd_showint16(0, 0, slave_encoder_left);
//        lcd_showint16(0, 1, slave_encoder_right);
//        lcd_showint16(0, 2, slave_position);
//        lcd_showint16(0, 3, time);

        lcd_showstr(0,0,"x_sum1:");
        lcd_showint16(12*8,0,sum_x1);
        lcd_showstr(0,1,"x_sum2:");
        lcd_showint16(12*8,1,sum_x2);
        lcd_showstr(0,2,"r_turn_flag:");
        lcd_showuint8(12*8,2,r_turn_flag);
        lcd_showstr(0,3,"l_turn_flag:");
        lcd_showuint8(12*8,3,l_turn_flag);
        lcd_showstr(0,4,"cross_flag:");
        lcd_showuint8(12*8,4,cross_flag);
//        lcd_showstr(0,2,"time:");
//        lcd_showint16(12*8, 2, slave_position);

    }
}
#endif
