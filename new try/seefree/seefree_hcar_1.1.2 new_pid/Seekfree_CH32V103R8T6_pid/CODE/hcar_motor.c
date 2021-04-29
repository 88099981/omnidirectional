/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00               东秦全向三队 编写                   00**
**00             队员 ：陈诏栋 刘坤龙 石柳苗                                                   00**
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00            函数作用：  motor                         00**
**0000000000000000000000000000000000000000000000000000000**
**********************************************************/
#include "hcar_motor.h"
#include "zf_gpio.h"
#include "zf_pwm.h"
#include "hcar_init.h"
#include "hcar_uart.h"

vuint8 S_i=0, S_j=0;
int16 speed_1[10]={0}, speed_2[10]={0};
int16 speed_sum1=0, speed_sum2=0;
int16 speed_3[10]={0}, speed_4[10]={0};
int16 speed_sum3=0, speed_sum4=0;

//pid_param_t pid1, pid2, pid3, pid4;
float speed_now_filter1=0.0, speed_now_filter2=0.0;
float speed_now_filter3=0.0, speed_now_filter4=0.0;
float speed_error1 = 0;
float speed_error_last1 = 0;
float speed_error_last_last1 = 0;
float speed_out1 = 0;
float speed_error2 = 0;
float speed_error_last2 = 0;
float speed_error_last_last2 = 0;
float speed_out2 = 0;
float speed_error3 = 0;
float speed_error_last3 = 0;
float speed_error_last_last3 = 0;
float speed_out3 = 0;
float speed_error4 = 0;
float speed_error_last4 = 0;
float speed_error_last_last4 = 0;
float speed_out4 = 0;

float kp1=1.5, ki1=0.36, kd1=0;
float kp2=1.5, ki2=0.36, kd2=0;
float kp3=1.5, ki3=0.36, kd3=0;
float kp4=1.5, ki4=0.36, kd4=0;
void hcar_motor_init(void)
{
    gpio_init(MOTOR1_A, GPO, 0, GPIO_PIN_CONFIG);
    pwm_init(MOTOR1_B,17000,0);
    gpio_init(MOTOR2_A, GPO, 0, GPIO_PIN_CONFIG);
    pwm_init(MOTOR2_B,17000,0);
    gpio_init(MOTOR3_A, GPO, 0, GPIO_PIN_CONFIG);
    pwm_init(MOTOR3_B,17000,0);
    gpio_init(MOTOR4_A, GPO, 0, GPIO_PIN_CONFIG);
    pwm_init(MOTOR4_B,17000,0);

//    hcar_PidInit(&pid1);
//    hcar_PidInit(&pid2);
//    hcar_PidInit(&pid3);
//    hcar_PidInit(&pid4);
//    /* PID 参数需要自行调节 */
//    pid1.kp = 7;
//    pid1.ki = 0;
//
//    pid2.kp = 7;
//    pid2.ki = 0;
//
//    pid3.kp = 7;
//    pid3.ki = 0;
//
//    pid4.kp = 7;
//    pid4.ki = 0;
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
void hcar_speed_control(int16 x, int16 y, int16 z)
{
    // 根据小车速度大小和方向  计算每个轮子的期望转速
    int16 realSpeed1=0, realSpeed2=0, realSpeed3=0, realSpeed4=0;
    targetSpeed1 =   x + y + z;
    targetSpeed2 =  -x + y - z;
    targetSpeed3 =   x + y - z;
    targetSpeed4 =  -x + y + z;

    // 获取编码器值
    realSpeed1 = host_encoder_left; //左电机 母板上编码器1，小车前进为负值
    realSpeed2 = host_encoder_right; //右电机 母板上编码器2，小车前进为正值
    realSpeed3 = slave_encoder_right; //另外一个核心板发数据过来
    realSpeed4 = slave_encoder_left; //另外一个核心板发数据过来
    //滤波
    speed_1[S_i] = realSpeed1;
    speed_2[S_i] = realSpeed2;
    speed_3[S_i] = realSpeed3;
    speed_4[S_i] = realSpeed4;
    //队列
    if(S_j<5)
    {
        S_j++;
    }
    S_i++;
    if(S_i > 4)
    {
        S_i = 0;
    }
    //累和归零
    speed_sum1 = 0;
    speed_sum2 = 0;
    speed_sum3 = 0;
    speed_sum4 = 0;
    //累和
    for(uint8_t j=0;j<5;j++)
    {
        speed_sum1 += speed_1[j];
    }
    for(uint8_t j=0;j<5;j++)
    {
        speed_sum2 += speed_2[j];
    }
    for(uint8_t j=0;j<5;j++)
    {
        speed_sum3 += speed_3[j];
    }
    for(uint8_t j=0;j<5;j++)
    {
        speed_sum4 += speed_4[j];
    }
    //平均
    speed_now_filter1 = speed_sum1/S_j;
    speed_now_filter2 = speed_sum2/S_j;
    speed_now_filter3 = speed_sum3/S_j;
    speed_now_filter4 = speed_sum4/S_j;
    // PID处理
//    hcar_PidIncCtrl(&pid1, targetSpeed1 - speed_now_filter1);
//    hcar_PidIncCtrl(&pid2, targetSpeed2 - speed_now_filter2);
//    hcar_PidIncCtrl(&pid3, targetSpeed3 - speed_now_filter3);
//    hcar_PidIncCtrl(&pid4, targetSpeed4 - speed_now_filter4);
    speed_error1 = (float)(targetSpeed1 - speed_now_filter1);
    speed_out1 += kp1 * (speed_error1 - speed_error_last1) + ki1 * speed_error1 +
                kd1*(speed_error1 - 2*speed_error_last1 + speed_error_last_last1);

    speed_error_last_last1 = speed_error_last1;
    speed_error_last1 = speed_error1;

    speed_error2 = (float)(targetSpeed2 - speed_now_filter2);
    speed_out2 += kp2 * (speed_error2 - speed_error_last2) + ki2 * speed_error2 +
                kd2*(speed_error2 - 2*speed_error_last2 + speed_error_last_last2);

    speed_error_last_last2 = speed_error_last2;
    speed_error_last2 = speed_error2;

    speed_error3 = (float)(targetSpeed3 - speed_now_filter3);
    speed_out3 += kp3 * (speed_error3 - speed_error_last3) + ki3 * speed_error3 +
                kd3*(speed_error3 - 2*speed_error_last3 + speed_error_last_last3);

    speed_error_last_last3 = speed_error_last3;
    speed_error_last3 = speed_error3;

    speed_error4 = (float)(targetSpeed4 - speed_now_filter4);
    speed_out4 += kp4 * (speed_error4 - speed_error_last4) + ki4 * speed_error4 +
                kd4*(speed_error4 - 2*speed_error_last4 + speed_error_last_last4);

    speed_error_last_last4 = speed_error_last4;
    speed_error_last4 = speed_error4;

    if(speed_out1>9000)
    {
        speed_out1=9000;
    }
    else if(speed_out1<-9000)
    {
        speed_out1=-9000;
    }
    if(speed_out2>9000)
    {
        speed_out2=9000;
    }
    else if(speed_out2<-9000)
    {
        speed_out2=-9000;
    }
    if(speed_out3>9000)
    {
        speed_out3=9000;
    }
    else if(speed_out3<-9000)
    {
        speed_out3=-9000;
    }
    if(speed_out4>9000)
    {
        speed_out4=9000;
    }
    else if(speed_out4<-9000)
    {
        speed_out4=-9000;
    }

    hcar_motor_control(speed_out1, speed_out2, speed_out3, speed_out4);


}

