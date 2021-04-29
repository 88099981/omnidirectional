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
    systick_delay_ms(1000);

    hcar_init();
    hcar_motor_init();
    gpio_init(BEEP_PIN,GPO,0,GPIO_PIN_CONFIG);//bb

    gpio_interrupt_init(A0, RISING, GPIO_INT_CONFIG);       //A0初始化为GPIO 上升沿触发
    nvic_init(EXTI0_IRQn, 1, 1, ENABLE);                    //EXTI0优先级配置，抢占优先级1，次优先级1

    uart_rx_irq(UART_3, ENABLE);                            //默认抢占优先级0 次优先级0。
    nvic_init((IRQn_Type)(53 + UART_3), 0, 0, ENABLE);      //将串口3的抢占优先级设置为最高，次优先级设置为最高。
    hcar_adc_init();//adc

    //seekfree_wireless_init();//无线串口

    icm20602_init_spi();//陀螺仪

    mt9v03x_init();//摄像头
    get_cut_array();//改变图片尺寸
    //aim_y=200;
    kp_z=KP_Z_SET;
    kd_z=KD_Z_SET;//分段需要去掉
    kp_ad=kp_ad_turn;
    kd_ad=kd_ad_turn;
    no_time=1100;
    //bb_time=100;
    EnableGlobalIRQ(0); //总中断最后开启

    hcar_start();

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    while(1)
    {
//        hcar_key_get();
//        //标志位置位之后，可以使用标志位执行自己想要做的事件

        if(mt9v03x_finish_flag==1)
        {
            mt9v03x_finish_flag = 0;
            //使用缩放显示函数，根据原始图像大小 以及设置需要显示的大小自动进行缩放或者放大显示
            //hcar_camera_wireless(DEBUG_UART, mt9v03x_image[0], MT9V03X_W, MT9V03X_H);

            //GPIO_PIN_SET(A4);
            cut_image_to_img();
            binary_img();
            get_new_edge2();

            //show_edge();
            get_middline();

            road_seek();
        //lcd_displayimage032_zoom(img[0], 80, 60, 160, 128);
            //GPIO_PIN_RESET(A4);
//共7ms
        }


        hcar_adc_get();
        //hcar_stop();
        aim_z=camera_speed-ad_speed;
        if(three_fork_flag==1&&ad_all<=240&&zuo_yuanhuan_flag==0&&you_yuanhuan_flag==0&&no_time<=0)
        {
            hcar_threefork30();
            bb_time=50;
            three_fork_flag=0;
            no_time=2150;
//            aim_y=0;
//            aim_z=200;
//            get_icm20602_gyro_spi();
//            turn_sum+=icm_gyro_z;
        }
        if(zebra_flag==1&&no_time1<=0)
        {
            zebra_first++;
            bb_time=100;
            if(zebra_first==1)
            {
                no_time1=1100;
                no_time=1100;
            }
            else if(zebra_first==2)
            {
                while(1)
                {
                    aim_x=0;
                    aim_y=0;
                    aim_z=0;
                    lcd_showstr(0,0,"aim_x:");
                    lcd_showint16(12*8,0,aim_x);
                    lcd_showstr(0,1,"aim_y:");
                    lcd_showint16(12*8,1,aim_y);
                    lcd_showstr(0,2,"aim_z:");
                    lcd_showint16(12*8,2,aim_z);
                }
            }
            zebra_flag=0;

        }
        //在TFT上显示测试变量

//        lcd_showstr(0,3,"icm_acc_x:");
//        lcd_showint16(12*8,3,icm_acc_x);
//        lcd_showstr(0,4,"icm_acc_y:");
//        lcd_showint16(12*8,4,icm_acc_y);
//        lcd_showstr(0,5,"icm_acc_z:");
//        lcd_showuint16(12*8,5,icm_acc_z);




//        lcd_showstr(0,1,"aim_y:");
//        lcd_showint16(12*8,1,aim_y);
//        lcd_showstr(0,2,"aim_z:");
//        lcd_showint16(12*8,2,aim_z);
//        lcd_showstr(0,3,"x_sum1:");
//        lcd_showint16(12*8,3,sum_x1);
//        lcd_showstr(0,4,"x_sum2:");
//        lcd_showint16(12*8,4,sum_x2);
//        lcd_showstr(0,3,"z_error:");
//        lcd_showint16(12*8,3,z_error);
//        lcd_showstr(0,3,"camera_speed:");
//        lcd_showint16(12*8,3,camera_speed);
//        lcd_showstr(0,4,"ad_speed:");
//        lcd_showint16(12*8,4,ad_speed);
//        lcd_showstr(0,5,"kp_z:");
//        lcd_showfloat(12*8,5,kp_z,2,1);
//        lcd_showstr(0,6,"kd_z:");
//        lcd_showfloat(12*8,6,kd_z,2,1);



        //将接收到的从机数据显示到屏幕上。
//        lcd_showstr(0, 0, "host_encoder_left:");
//        lcd_showint16(0, 1, host_encoder_left);
//        lcd_showstr(0, 2, "host_encoder_right:");
//        lcd_showint16(0, 3, host_encoder_right);
//        lcd_showstr(0, 4, "slave_encoder_right:");
//        lcd_showint16(0, 5, slave_encoder_right);
//        lcd_showstr(0, 6, "slave_encoder_left:");
//        lcd_showint16(0, 7, slave_encoder_left);
//        lcd_showstr(0,1,"ad_speed:");
//        lcd_showint16(12*8,1,ad_speed);


        lcd_showstr(0,0,"zyh_flag:");
        lcd_showuint8(12*8,0,zuo_yuanhuan_flag);
        lcd_showstr(0,1,"yyh_flag:");
        lcd_showuint8(12*8,1,you_yuanhuan_flag);
        lcd_showstr(0,2,"ad_value1:");
        lcd_showuint16(12*8,2,ad_value1);
        lcd_showstr(0,3,"judge_lr:");
        lcd_showint16(12*8,3,judge_lr);
        lcd_showstr(0,4,"fuhe_ad_all:");
        lcd_showint16(12*8,4,fuhe_ad_all);
        lcd_showstr(0,5,"ad_value4:");
        lcd_showuint16(12*8,5,ad_value4);
        lcd_showstr(0,6,"ad_all:");
        lcd_showuint16(12*8,6,ad_all);
        lcd_showstr(0,7,"ad_error:");
        lcd_showfloat(12*8,7,ad_error,3,2);



//        lcd_showstr(0,0,"sancha:");
//        lcd_showuint8(12*8,0,three_fork_flag);
//        lcd_showstr(0,1,"fuhe_sancha:");
//        lcd_showuint8(12*8,1,fuhe_sancha);
//        lcd_showstr(0,1,"shizi:");
//        lcd_showuint8(12*8,1,cross_flag);



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
    systick_delay_ms(1000);
    hcar_init();

    gpio_init(A0, GPO, 0, GPIO_PIN_CONFIG);                 //同步引脚初始化
    timer_pit_interrupt_ms(TIMER_4, 20);                    //定时器4初始化
    //gpio_init(BEEP_PIN,GPO,0,GPIO_PIN_CONFIG);//bb

//    seekfree_wireless_init();
//    uint8 test[]="This is wireless application.";
//    seekfree_wireless_send_buff(test, sizeof(test)-1);
//    mt9v03x_init();//摄像头
//    get_cut_array();//改变图片尺寸
    //aim_y=200;
    kp_ad=1.5;
    kd_ad=5.0;//分段需要去掉
    hcar_adc_init();//adc
    EnableGlobalIRQ(0);
    while(1)
    {
//        if(mt9v03x_finish_flag==1)
//        {
//            mt9v03x_finish_flag = 0;
//            //使用缩放显示函数，根据原始图像大小 以及设置需要显示的大小自动进行缩放或者放大显示
//            //hcar_camera_wireless(DEBUG_UART, mt9v03x_image[0], MT9V03X_W, MT9V03X_H);
//
//            //GPIO_PIN_SET(A4);
//            cut_image_to_img();
//            binary_img();
//
//            get_new_edge();
//            set_middline_black();
//            //three_fork();
//            road_seek();
//            lcd_displayimage032_zoom(img[0], 80, 60, 160, 128);
//            //GPIO_PIN_RESET(A4);
////共7ms
//        }
        three_fork_flag=0;
        ad_value1 = adc_mean_filter(ADC_CH1, ADC_10BIT, 5);
        ad_value2 = adc_mean_filter(ADC_CH2, ADC_10BIT, 5);
        ad_value3 = adc_mean_filter(ADC_CH3, ADC_10BIT, 5);
        ad_value4 = adc_mean_filter(ADC_CH4, ADC_10BIT, 5);

        ad_all=ad_value4+ad_value3+ad_value2+ad_value1;

        ad_left=ad_value1*0.7+ad_value2*0.3;
        ad_right=ad_value3*0.3+ad_value4*0.7;

        ad_error=(ad_left-ad_right)*100/(ad_left+ad_right);
        ad_speed= kp_ad*ad_error + kd_ad*(ad_error-lasttime_ad);
        lasttime_ad=ad_error;
        if(ad_value4+ad_value3+ad_value2+ad_value1>180)
        {
            three_fork_flag=1;
            //bb_time=5;
        }
        //将接收到的从机数据显示到屏幕上。
//        lcd_showint16(0, 0, slave_encoder_left);
//        lcd_showint16(0, 1, slave_encoder_right);
        //lcd_showint16(0, 2, slave_position);
//        lcd_showstr(0,2,"sancha:");
//        lcd_showuint8(12*8,2,three_fork_flag);
        lcd_showstr(0,0,"ad_speed:");
        lcd_showint16(12*8,0,ad_speed);
        lcd_showstr(0,2,"ad_value1:");
        lcd_showuint16(12*8,2,ad_value1);
        lcd_showstr(0,3,"ad_value2:");
        lcd_showuint16(12*8,3,ad_value2);
        lcd_showstr(0,4,"ad_value3:");
        lcd_showuint16(12*8,4,ad_value3);
        lcd_showstr(0,5,"ad_value4:");
        lcd_showuint16(12*8,5,ad_value4);
        lcd_showstr(0,6,"ad_all:");
        lcd_showuint16(12*8,6,ad_all);
        lcd_showstr(0,7,"sancha:");
        lcd_showuint8(12*8,7,three_fork_flag);



    }
}
#endif
