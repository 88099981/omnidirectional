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
#include "Serial_oscilloscope.h"

#define MOTOR1_A   C10              //定义1电机正反转引脚
#define MOTOR1_B   PWM4_CH1_B6      //定义1电机PWM引脚

#define MOTOR2_A   C11               //定义2电机正反转引脚
#define MOTOR2_B   PWM4_CH2_B7      //定义2电机PWM引脚

#define MOTOR3_A   B12              //定义3电机正反转引脚
#define MOTOR3_B   PWM4_CH3_B8      //定义3电机PWM引脚

#define MOTOR4_A   A8              //定义4电机正反转引脚
#define MOTOR4_B   PWM4_CH4_B9      //定义4电机PWM引脚

#define KEY1    C8
#define KEY2    C9
#define KEY3    B2
vuint8 key1_status = 1;
vuint8 key2_status = 1;
vuint8 key3_status = 1;

//上一次开关状态变量
vuint8 key1_last_status;
vuint8 key2_last_status;
vuint8 key3_last_status;

//开关标志位
vuint8 key1_flag;
vuint8 key2_flag;
vuint8 key3_flag;

uint16 encoder_str[20];
int16 host_encoder_left=0;
int16 host_encoder_right=0;
int16 duty=2000;

void hcar_pid_wireless();
void hcar_motor_control(int16, int16, int16, int16);

int main(void)
{
    DisableGlobalIRQ();
    board_init();           //务必保留，本函数用于初始化MPU 时钟 调试串口

    gpio_init(KEY1, GPI, 0, GPIO_INT_CONFIG);
    gpio_init(KEY2, GPI, 0, GPIO_INT_CONFIG);
    gpio_init(KEY3, GPI, 0, GPIO_INT_CONFIG);



    //uint8 sign=-5;
    seekfree_wireless_init();//无线串口
    lcd_init();
    gpio_init(MOTOR1_A, GPO, 0, GPIO_PIN_CONFIG);
    pwm_init(MOTOR1_B,17000,0);
    gpio_init(MOTOR2_A, GPO, 0, GPIO_PIN_CONFIG);
    pwm_init(MOTOR2_B,17000,0);
    gpio_init(MOTOR3_A, GPO, 0, GPIO_PIN_CONFIG);
    pwm_init(MOTOR3_B,17000,0);
    gpio_init(MOTOR4_A, GPO, 0, GPIO_PIN_CONFIG);
    pwm_init(MOTOR4_B,17000,0);
    //uart_init(UART_3, 460800, UART3_TX_B10, UART3_RX_B11);  //串口3初始化，波特率460800
    timer_quad_init(TIMER_2, TIMER2_CHA_A15, TIMER2_CHB_B3);
    timer_quad_init(TIMER_3, TIMER3_CHA_B4, TIMER3_CHB_B5);//两个编码器
    timer_pit_interrupt_ms(TIMER_4, 20);                    //定时器4初始化
    EnableGlobalIRQ(0);

    while(1)
    {

//        key1_last_status = key1_status;
//        key2_last_status = key2_status;
//        key3_last_status = key3_status;
//
//        //读取当前按键状态
//        key1_status = gpio_get(KEY1);
//        key2_status = gpio_get(KEY2);
//        key3_status = gpio_get(KEY3);
//
//
//        //检测到按键按下之后  并放开置位标志位
//        if(key1_status && !key1_last_status)    key1_flag = 1;
//        if(key2_status && !key2_last_status)    key2_flag = 1;
//        if(key3_status && !key3_last_status)    key3_flag = 1;
//        if(key1_flag)
//        {
//            key1_flag = 0;//使用按键之后，应该清除标志位
//            duty-=50;
//        }
//
//        if(key2_flag)
//        {
//            key2_flag = 0;//使用按键之后，应该清除标志位
//            duty=100;
//        }
//
//        if(key3_flag)
//        {
//            key3_flag = 0;//使用按键之后，应该清除标志位
//            duty+=50;
//        }
//        lcd_showstr(0,1,"duty:");
//        lcd_showint16(12*8,1,duty);


//        lcd_showstr(0,1,"number:");
//        lcd_showint16(12*8,1,host_encoder_right);
//        host_encoder_right=-timer_quad_get(TIMER_3);//已校准

        //timer_quad_clear(TIMER_3);                  //清空计数器


    }
}

void hcar_motor_control(int16 motor1, int16 motor2, int16 motor3, int16 motor4)
{
    motor2=-motor2;
    motor3=-motor3;
    if (motor1 > 0)
    {
        gpio_set(MOTOR1_A, 0);
        pwm_duty(MOTOR1_B, motor1);
    }
    else
    {
        gpio_set(MOTOR1_A, 1);
        pwm_duty(MOTOR1_B, PWM_DUTY_MAX+motor1);
    }

    if (motor2 > 0)
    {
        gpio_set(MOTOR2_A, 0);
        pwm_duty(MOTOR2_B, motor2);
    }
    else
    {
        gpio_set(MOTOR2_A, 1);
        pwm_duty(MOTOR2_B, PWM_DUTY_MAX+motor2);
    }

    if (motor3 > 0)
    {
        gpio_set(MOTOR3_A, 0);
        pwm_duty(MOTOR3_B, motor3);
    }
    else
    {
        gpio_set(MOTOR3_A, 1);
        pwm_duty(MOTOR3_B, PWM_DUTY_MAX+motor3);
    }

    if (motor4 > 0)
    {
        gpio_set(MOTOR4_A, 0);
        pwm_duty(MOTOR4_B, motor4);
    }
    else
    {
        gpio_set(MOTOR4_A, 1);
        pwm_duty(MOTOR4_B, PWM_DUTY_MAX+motor4);
    }
}

void hcar_pid_wireless(void)
{
    encoder_str[0]=host_encoder_left;
    encoder_str[1]=duty;
//    encoder_str[2]=slave_encoder_right;
//    encoder_str[3]=slave_encoder_left;
//    encoder_str[4]=targetSpeed1;
//    encoder_str[5]=targetSpeed2;
//    encoder_str[6]=targetSpeed3;
//    encoder_str[7]=targetSpeed4;
    //encoder_str[8]=(int16)sum_x1*100;
    Data_Send(WIRELESS_UART, encoder_str);
}
