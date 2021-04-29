#ifndef _hcar_motor_h
#define _hcar_motor_h

#include "common.h"

#define MOTOR1_A   C10              //定义1电机正反转引脚
#define MOTOR1_B   PWM4_CH1_B6      //定义1电机PWM引脚

#define MOTOR2_A   C11               //定义2电机正反转引脚
#define MOTOR2_B   PWM4_CH2_B7      //定义2电机PWM引脚

#define MOTOR3_A   B12              //定义3电机正反转引脚
#define MOTOR3_B   PWM4_CH3_B8      //定义3电机PWM引脚

#define MOTOR4_A   A8              //定义4电机正反转引脚
#define MOTOR4_B   PWM4_CH4_B9      //定义4电机PWM引脚

//int32 speed1_power;
//int32 speed2_power;
//int32 speed3_power;
//int32 speed4_power;
//函数
void hcar_motor_init(void);
void hcar_motor_control(int16 motor1, int16 motor2, int16 motor3, int16 motor4);
void hcar_speed_control(int16 x, int16 y, int16 z);

#endif
