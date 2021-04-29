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
#include "hcar_gogogo.h"
#include "headfile.h"

#ifdef DOUBLE_CORE_HOST
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////双机通信之主机，完成整体计算，插在右边卡槽
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void hcar_gogogo(void)//双机通信之主机，完成整体计算，插在右边卡槽
{

  // 系统及延时函数初始化
    DisableGlobalIRQ();
    board_init();           //务必保留，本函数用于初始化MPU 时钟 调试串口
    systick_delay_ms(300);

    hcar_init();
    hcar_motor_init();

    gpio_interrupt_init(A0, RISING, GPIO_INT_CONFIG);       //A0初始化为GPIO 上升沿触发
    nvic_init(EXTI0_IRQn, 1, 1, ENABLE);                    //EXTI0优先级配置，抢占优先级1，次优先级1

    uart_rx_irq(UART_3, ENABLE);                            //默认抢占优先级0 次优先级0。
    nvic_init((IRQn_Type)(53 + UART_3), 0, 0, ENABLE);      //将串口3的抢占优先级设置为最高，次优先级设置为最高。
    hcar_adc_init();//adc

    //seekfree_wireless_init();//无线串口

    //icm20602_init_spi();//陀螺仪

    mt9v03x_init();//摄像头
    get_cut_array();//改变图片尺寸

    kp_z=KP_Z_SET;
    kd_z=KD_Z_SET;//分段需要去掉

    EnableGlobalIRQ(0); //总中断最后开启

    //hcar_start();

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    while(1)
    {
        //hcar_key_get();
//        //标志位置位之后，可以使用标志位执行自己想要做的事件
//        if(key1_flag)
//        {
//            key1_flag = 0;//使用按键之后，应该清除标志位
//            aim_y-=100;
//        }
//
//        if(key2_flag)
//        {
//            key2_flag = 0;//使用按键之后，应该清除标志位
//            aim_y=300;
//        }
//
//        if(key3_flag)
//        {
//            key3_flag = 0;//使用按键之后，应该清除标志位
//            aim_y+=100;
//        }

        //hcar_adc_get();
//        hcar_stop();
        //aim_y=200;
        if(mt9v03x_finish_flag==1&&turn_sign==0)
        {
            mt9v03x_finish_flag = 0;
            //使用缩放显示函数，根据原始图像大小 以及设置需要显示的大小自动进行缩放或者放大显示
            //hcar_camera_wireless(DEBUG_UART, mt9v03x_image[0], MT9V03X_W, MT9V03X_H);

            //three_fork();
            cut_image_to_img();
            binary_img();
            //zebra_crossing();
            get_new_edge();
            set_middline_black();
            road_seek();
            lcd_displayimage032_zoom(img[0], 80, 60, 160, 128);

        }
        //get_icm20602_accdata_spi();
        //get_icm20602_gyro_spi();
        //在TFT上显示测试变量
//        lcd_showstr(0,0,"icm_gyro_x:");
//        lcd_showint16(12*8,0,icm_gyro_x);
//        lcd_showstr(0,1,"icm_gyro_y:");
//        lcd_showint16(12*8,1,icm_gyro_y);
//        lcd_showstr(0,2,"icm_gyro_z:");
//        lcd_showint16(12*8,2,icm_gyro_z);


//        lcd_showstr(0,0,"aim_x:");
//        lcd_showint16(12*8,0,aim_x);
//        lcd_showstr(0,1,"aim_y:");
//        lcd_showint16(12*8,1,aim_y);
//        lcd_showstr(0,2,"aim_z:");
//        lcd_showint16(12*8,2,aim_z);
//        lcd_showstr(0,3,"x_sum1:");
//        lcd_showint8(12*8,3,sum_x1);
//        lcd_showstr(0,4,"x_sum2:");
//        lcd_showint8(12*8,4,sum_x2);
//        lcd_showstr(0,5,"z_error:");
//        lcd_showint16(12*8,5,z_error);
//        lcd_showstr(0,6,"r_turn_flag:");
//        lcd_showint8(12*8,6,r_turn_flag);
//        lcd_showstr(0,7,"l_turn_flag:");
//        lcd_showint8(12*8,7,l_turn_flag);



        //将接收到的从机数据显示到屏幕上。
//        lcd_showstr(0, 0, "host_encoder_left:");
//        lcd_showint16(0, 1, host_encoder_left);
//        lcd_showstr(0, 2, "host_encoder_right:");
//        lcd_showint16(0, 3, host_encoder_right);
//        lcd_showstr(0, 4, "slave_encoder_right:");
//        lcd_showint16(0, 5, slave_encoder_right);
//        lcd_showstr(0, 6, "slave_encoder_left:");
//        lcd_showint16(0, 7, slave_encoder_left);

//        lcd_showstr(0,2,"guiyi_ad1:");
//        lcd_showuint16(12*8,2,guiyi_ad1);
//        lcd_showstr(0,3,"guiyi_ad2:");
//        lcd_showuint16(12*8,3,guiyi_ad2);
//        lcd_showstr(0,4,"guiyi_ad3:");
//        lcd_showuint16(12*8,4,guiyi_ad3);
//        lcd_showstr(0,5,"guiyi_ad4:");
//        lcd_showuint16(12*8,5,guiyi_ad4);
//        lcd_showstr(0,2,"ad_value1:");
//        lcd_showuint16(12*8,2,ad_value1);
//        lcd_showstr(0,3,"ad_value2:");
//        lcd_showuint16(12*8,3,ad_value2);
//        lcd_showstr(0,4,"ad_value3:");
//        lcd_showuint16(12*8,4,ad_value3);
//        lcd_showstr(0,5,"ad_value4:");
//        lcd_showuint16(12*8,5,ad_value4);

//        lcd_showstr(0,6,"ad_error:");
//        lcd_showint16(12*8,6,ad_error);


    }
}

#elif  defined DOUBLE_CORE_SLAVE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////双机通信之从机，发送编码器和或摄像头算法结果数据，插在左边卡槽
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void hcar_gogogo(void)//双机通信之从机，发送编码器和或摄像头算法结果数据，插在左边卡槽
{


    // 系统及延时函数初始化
    DisableGlobalIRQ();
    board_init();           //务必保留，本函数用于初始化MPU 时钟 调试串口
    hcar_init();

    gpio_init(A0, GPO, 0, GPIO_PIN_CONFIG);                 //同步引脚初始化
    timer_pit_interrupt_ms(TIMER_4, 20);                    //定时器4初始化

//    seekfree_wireless_init();
//    uint8 test[]="This is wireless application.";
//    seekfree_wireless_send_buff(test, sizeof(test)-1);

    EnableGlobalIRQ(0);
    while(1)
    {

        //将接收到的从机数据显示到屏幕上。
        lcd_showint16(0, 0, slave_encoder_left);
        lcd_showint16(0, 1, slave_encoder_right);
        //lcd_showint16(0, 2, slave_position);


    }
}
#endif
